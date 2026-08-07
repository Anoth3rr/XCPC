from __future__ import annotations

import html
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from pypdf import PdfReader, PdfWriter


ROOT = Path(__file__).resolve().parent.parent
PRINT = ROOT / "00 - 打印稿整理"
TMP = ROOT / "tmp" / "pdfs"
OUT = ROOT / "output" / "pdf"
GEN = PRINT / "生成打印稿.py"
RENDER = PRINT / "渲染打印稿.cjs"
MD = PRINT / "00 - XCPC全量打印稿.md"
MAP = PRINT / "页码映射.json"
INDEX = PRINT / "页码索引.md"
FINAL = OUT / "林月的XCPC算法模板-v1.0.0.pdf"

OWNER = "林月"
VERSION = "v1.0.0"
UPDATED = "2026.8.1"


def first_existing(candidates: list[Path], name: str) -> Path:
    for p in candidates:
        if p.is_file():
            return p
    found = shutil.which(name)
    if found:
        return Path(found)
    raise FileNotFoundError(f"找不到 {name}：{candidates}")


def ensure_node_packages() -> Path:
    modules = PRINT / "node_modules"
    wanted = [modules / "katex" / "dist" / "katex.js", modules / "marked" / "lib" / "marked.umd.js"]
    if all(p.is_file() for p in wanted):
        return modules
    npm = first_existing(
        [Path(os.environ.get("PROGRAMFILES", r"C:\Program Files")) / "nodejs" / "npm.cmd"],
        "npm.cmd",
    )
    run([str(npm), "ci", "--ignore-scripts", "--no-audit", "--no-fund"], cwd=PRINT)
    if not all(p.is_file() for p in wanted):
        raise RuntimeError("npm ci 完成后仍找不到 KaTeX / marked")
    return modules


def tools() -> tuple[Path, Path, list[Path]]:
    dep = Path.home() / ".cache" / "codex-runtimes" / "codex-primary-runtime" / "dependencies"
    node = first_existing([dep / "node" / "bin" / "node.exe"], "node")
    chrome = first_existing(
        [
            Path(os.environ.get("PROGRAMFILES", r"C:\Program Files")) / "Google" / "Chrome" / "Application" / "chrome.exe",
            Path(os.environ.get("PROGRAMFILES(X86)", r"C:\Program Files (x86)")) / "Microsoft" / "Edge" / "Application" / "msedge.exe",
        ],
        "chrome",
    )
    bundled = dep / "node" / "node_modules"
    if not bundled.is_dir():
        raise FileNotFoundError(f"找不到 Node 模块目录：{bundled}")
    local = ensure_node_packages()
    return node, chrome, [local, bundled]


def run(args: list[str], env: dict[str, str] | None = None, cwd: Path = ROOT) -> None:
    print("+", subprocess.list2cmdline(args), flush=True)
    subprocess.run(args, cwd=cwd, env=env, check=True)


def render_pdf(node: Path, chrome: Path, modules: list[Path], pdf: Path, profile: Path) -> int:
    env = os.environ.copy()
    env["NODE_PATH"] = os.pathsep.join(map(str, modules))
    html_file = TMP / "林月的XCPC算法模板.html"
    run([str(node), str(RENDER), str(MD), str(html_file)], env)
    rendered = html_file.read_text(encoding="utf-8")
    match = re.search(r'<meta name="math-count" content="(\d+)">', rendered)
    math_count = int(match.group(1)) if match else 0
    if math_count <= 0 or 'class="katex' not in rendered:
        raise RuntimeError(f"HTML 数学公式验收失败：math_count={math_count}")
    if pdf.exists():
        pdf.unlink()
    run(
        [
            str(chrome),
            "--headless=new",
            "--disable-gpu",
            "--allow-file-access-from-files",
            "--no-pdf-header-footer",
            f"--user-data-dir={profile}",
            f"--print-to-pdf={pdf}",
            html_file.as_uri(),
        ]
    )
    if not pdf.is_file() or pdf.stat().st_size == 0:
        raise RuntimeError("Chromium 未生成 PDF")
    return math_count


def expected_keys() -> list[str]:
    text = MD.read_text(encoding="utf-8")
    keys = re.findall(r"LYPAGE:([a-z0-9-]+):END", text)
    if len(keys) != len(set(keys)):
        dup = sorted(k for k in set(keys) if keys.count(k) > 1)
        raise RuntimeError(f"页码标记重复：{dup}")
    return keys


def extract_map(pdf: Path, keys: list[str]) -> dict[str, int]:
    wanted = set(keys)
    result: dict[str, int] = {}
    for page_no, page in enumerate(PdfReader(pdf).pages, 1):
        text = re.sub(r"\s+", "", page.extract_text() or "")
        for key in re.findall(r"LYPAGE:([a-z0-9-]+):END", text):
            if key not in wanted:
                continue
            if key in result:
                raise RuntimeError(f"页码标记重复出现在 PDF：{key}")
            result[key] = page_no
    missing = sorted(wanted - result.keys())
    if missing:
        raise RuntimeError(f"PDF 未提取到 {len(missing)} 个页码标记：{missing[:10]}")
    return result


def write_map(data: dict[str, int]) -> None:
    MAP.write_text(json.dumps(data, ensure_ascii=False, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")


def entries(source: str, page_map: dict[str, int]) -> list[tuple[str, str, str, int]]:
    items: list[tuple[str, str, str, int]] = []
    current = ""
    part_pat = re.compile(r'<section class="part-cover" id="([^"]+)">.*?<h1>(.*?)</h1>', re.S)
    starts = list(part_pat.finditer(source))
    for i, match in enumerate(starts):
        current = html.unescape(re.sub(r"<[^>]+>", "", match.group(2)))
        key = match.group(1)
        items.append((key, "分册", current, page_map[key]))
        end = starts[i + 1].start() if i + 1 < len(starts) else len(source)
        block = source[match.end() : end]
        for h in re.finditer(r'<h2 id="([^"]+)" class="(doc-title|template-title)"><code>(.*?)</code>.*?</h2>', block, re.S):
            key, cls, title = h.groups()
            kind = "知识" if cls == "doc-title" else "代码"
            title = html.unescape(re.sub(r"<[^>]+>", "", title))
            items.append((key, kind, f"{current} / {title}", page_map[key]))
    return items


def add_metadata_and_outlines(source_pdf: Path, final_pdf: Path, page_map: dict[str, int], items: list[tuple[str, str, str, int]]) -> None:
    reader = PdfReader(source_pdf)
    writer = PdfWriter()
    writer.clone_document_from_reader(reader)
    writer.add_metadata(
        {
            "/Title": f"林月的 XCPC 算法模板 {VERSION}",
            "/Author": OWNER,
            "/Subject": f"XCPC / NOI 竞赛算法模板；更新时间 {UPDATED}",
            "/Keywords": f"林月, XCPC, NOI, 算法模板, {VERSION}",
            "/Creator": "林月的 XCPC 模板库生成器",
            "/Producer": "Chromium + pypdf",
            "/CreationDate": "D:20260801000000+08'00'",
            "/ModDate": "D:20260801000000+08'00'",
        }
    )
    parent = None
    for key, kind, title, page in items:
        if kind == "分册":
            parent = writer.add_outline_item(title, page - 1)
        else:
            short = title.split(" / ", 1)[-1]
            writer.add_outline_item(short, page - 1, parent=parent)
    with final_pdf.open("wb") as f:
        writer.write(f)


def write_index(page_count: int, items: list[tuple[str, str, str, int]]) -> None:
    lines = [
        "# 林月的 XCPC 算法模板页码索引",
        "",
        f"> 所有者：{OWNER}；版本：{VERSION}；更新时间：{UPDATED}；PDF：{page_count} 页。",
        "",
        "页码均为最终 PDF 的物理页码，与纸面页脚中央页码一致。此文件由 `生成最终PDF.py` 自动生成。",
        "",
        "|类型|条目|页码|",
        "|---|---|---:|",
    ]
    for _, kind, title, page in items:
        lines.append(f"|{kind}|{title.replace('|', '\\|')}|{page}|")
    INDEX.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")


def verify(final_pdf: Path, page_map: dict[str, int], items: list[tuple[str, str, str, int]]) -> int:
    reader = PdfReader(final_pdf)
    pages = len(reader.pages)
    meta = reader.metadata
    if meta.author != OWNER or VERSION not in (meta.title or ""):
        raise RuntimeError(f"PDF 元数据错误：author={meta.author!r}, title={meta.title!r}")
    if any(not 1 <= p <= pages for p in page_map.values()):
        raise RuntimeError("页码映射超出 PDF 范围")
    source = MD.read_text(encoding="utf-8")
    for key, page in page_map.items():
        token = f'<span class="toc-page">{page}</span>'
        if token not in source:
            raise RuntimeError(f"Markdown 未回填页码：{key} -> {page}")
    if len(items) != len(page_map):
        raise RuntimeError(f"书签条目与页码标记数量不一致：{len(items)} != {len(page_map)}")
    return pages


def main() -> None:
    TMP.mkdir(parents=True, exist_ok=True)
    OUT.mkdir(parents=True, exist_ok=True)
    node, chrome, modules = tools()
    old: dict[str, int] = {}
    if MAP.exists():
        old = json.loads(MAP.read_text(encoding="utf-8"))
    # OneDrive 可能在 Chrome 退出后短暂占用配置目录；不让临时目录清理影响成品。
    with tempfile.TemporaryDirectory(prefix="chrome-", dir=TMP, ignore_cleanup_errors=True) as profile:
        converged_pdf: Path | None = None
        for turn in range(1, 7):
            run([sys.executable, str(GEN)])
            keys = expected_keys()
            pass_pdf = TMP / f"pass-{turn}.pdf"
            math_count = render_pdf(node, chrome, modules, pass_pdf, Path(profile))
            new = extract_map(pass_pdf, keys)
            print(
                f"pass {turn}: pages={len(PdfReader(pass_pdf).pages)}, "
                f"markers={len(new)}, math={math_count}",
                flush=True,
            )
            if new == old:
                converged_pdf = pass_pdf
                break
            write_map(new)
            old = new
        if converged_pdf is None:
            raise RuntimeError("目录页码在 6 轮内未收敛")

        source = MD.read_text(encoding="utf-8")
        items = entries(source, old)
        add_metadata_and_outlines(converged_pdf, FINAL, old, items)
        pages = verify(FINAL, old, items)
        write_index(pages, items)

    for p in TMP.iterdir():
        try:
            if p.is_dir():
                shutil.rmtree(p)
            else:
                p.unlink()
        except OSError:
            pass
    print(f"final={FINAL}; pages={pages}; indexed={len(old)}; owner={OWNER}; version={VERSION}; updated={UPDATED}")


if __name__ == "__main__":
    main()
