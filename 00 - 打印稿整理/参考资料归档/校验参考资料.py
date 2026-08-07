from __future__ import annotations

import hashlib
import json
import re
import zipfile
from pathlib import Path, PurePosixPath

from pypdf import PdfReader


HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent
RAW = HERE / "原始文件"
EXPANDED = HERE / "展开"

EXPECTED = {
    "WIDA-XCPC-v1.8.8-2024.11.19.md": (403490, "48059896e7bdbb9b1000cd0171e717a81ff7bc5c05b71100b76cc520711c433d"),
    "XCPC-1.8.8.zip": (1205018, "836c1f8d81f356448a519616be1ece9020fc8e2315055c5f84c3a1320c79991b"),
    "WIDA-XCPC-v1.8.8-2024.11.19.pdf": (6951596, "f6aa842f7c24848a0a1afc7ffaa2daf206f623bfad9558a9f7518f8d7fe2981e"),
}


def sha256(p: Path) -> str:
    h = hashlib.sha256()
    with p.open("rb") as f:
        for b in iter(lambda: f.read(1 << 20), b""):
            h.update(b)
    return h.hexdigest()


def markdown_headings(p: Path) -> tuple[int, int]:
    h2 = total = 0
    fenced = False
    for line in p.read_text(encoding="utf-8-sig").splitlines():
        if line.startswith("```"):
            fenced = not fenced
            continue
        if fenced:
            continue
        m = re.match(r"^(#{1,6})\s+", line)
        if m:
            total += 1
            h2 += len(m.group(1)) == 2
    return h2, total


def main() -> None:
    hashes = {}
    for name, (size, want) in EXPECTED.items():
        p = RAW / name
        got = sha256(p)
        if p.stat().st_size != size or got != want:
            raise RuntimeError(f"原件不一致：{name}; size={p.stat().st_size}; sha256={got}")
        hashes[name] = got

    zpath = RAW / "XCPC-1.8.8.zip"
    count = 0
    with zipfile.ZipFile(zpath) as z:
        for x in z.infolist():
            if x.is_dir():
                continue
            rel = PurePosixPath(x.filename)
            if rel.is_absolute() or ".." in rel.parts:
                raise RuntimeError(f"ZIP 非法路径：{x.filename}")
            p = EXPANDED / rel
            if not p.is_file() or p.read_bytes() != z.read(x):
                raise RuntimeError(f"ZIP 展开内容不一致：{x.filename}")
            count += 1
    files = [p for p in EXPANDED.rglob("*") if p.is_file()]
    if count != 190 or len(files) != 190:
        raise RuntimeError(f"ZIP 文件数异常：zip={count}; expanded={len(files)}")

    h2, headings = markdown_headings(RAW / "WIDA-XCPC-v1.8.8-2024.11.19.md")
    pages = len(PdfReader(RAW / "WIDA-XCPC-v1.8.8-2024.11.19.pdf").pages)
    if (h2, headings, pages) != (14, 438, 232):
        raise RuntimeError(f"参考结构异常：h2={h2}; headings={headings}; pages={pages}")

    outside = [p for p in ROOT.rglob("*") if p.is_file() and HERE not in p.parents]
    if not outside:
        raise RuntimeError("最终项目没有归档外文件，无法构成严格超集")

    print(
        json.dumps(
            {
                "strict_subset": True,
                "owner": "林月",
                "version": "v1.0.0",
                "updated": "2026.8.1",
                "raw_sha256": hashes,
                "zip_files_verified": count,
                "reference_markdown_sections": h2,
                "reference_markdown_headings": headings,
                "reference_pdf_pages": pages,
                "final_files_outside_archive": len(outside),
            },
            ensure_ascii=False,
            indent=2,
        )
    )


if __name__ == "__main__":
    main()
