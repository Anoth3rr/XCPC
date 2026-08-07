from __future__ import annotations

import html
import json
import os
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Callable
from urllib.parse import quote, unquote


ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "00 - 打印稿整理"
OWNER = "林月"
VERSION = "v1.0.0"
UPDATED = "2026.8.1"
PAGE_MAP_FILE = OUT / "页码映射.json"
LOCAL_INDEX = ROOT / "模板索引.md"
FINAL_PDF = ROOT / "output" / "pdf" / "林月的XCPC算法模板-v1.0.0.pdf"


def load_page_map() -> dict[str, int]:
    if not PAGE_MAP_FILE.exists():
        return {}
    data = json.loads(PAGE_MAP_FILE.read_text(encoding="utf-8"))
    if not isinstance(data, dict) or any(not isinstance(k, str) or not isinstance(v, int) for k, v in data.items()):
        raise RuntimeError(f"非法页码映射：{PAGE_MAP_FILE}")
    return data


PAGE_MAP = load_page_map()


@dataclass(frozen=True)
class Part:
    out: str
    title: str
    folder: str
    pick: Callable[[Path], bool]
    docs: tuple[str, ...] = ()
    exts: tuple[str, ...] = (".cpp",)


def num(lo: int, hi: int) -> Callable[[Path], bool]:
    def pick(p: Path) -> bool:
        m = re.match(r"^(\d+)", p.name)
        return bool(m and lo <= int(m.group(1)) <= hi)

    return pick


def all_code(_: Path) -> bool:
    return True


PARTS = [
    Part(
        "01 - 基础算法.md",
        "基础算法",
        "01 - 基础算法",
        all_code,
        ("README.md", "基础算法与STL速查.md"),
        (".cpp", ".hpp"),
    ),
    Part(
        "02A - 树上问题（静态树与分治）.md",
        "树上问题：静态树、分治与离线",
        "02 - 树上问题",
        num(1, 9),
        ("README.md", "树上问题知识与结论.md"),
    ),
    Part(
        "02B - 树上问题（高级与动态）.md",
        "树上问题：重构树、动态树与编码",
        "02 - 树上问题",
        num(10, 13),
    ),
    Part(
        "03A - 数据结构（基础与区间）.md",
        "数据结构：并查集、区间结构与 RMQ",
        "03 - 数据结构",
        num(1, 5),
        ("README.md", "数据结构知识与结论.md"),
    ),
    Part(
        "03B - 数据结构（顺序统计与平衡树）.md",
        "数据结构：顺序统计、Trie、堆、平衡树与哈希",
        "03 - 数据结构",
        num(6, 13),
    ),
    Part(
        "04A - 图论（路径、生成树与连通）.md",
        "图论：存图、路径、生成树与连通性",
        "04 - 图论",
        num(0, 5),
        ("README.md", "图论知识与结论.md"),
    ),
    Part(
        "04B - 图论（匹配与全局割）.md",
        "图论：匹配与全局最小割",
        "04 - 图论",
        num(6, 9),
    ),
    Part(
        "05 - 网络流.md",
        "网络流",
        "05 - 网络流",
        all_code,
        ("README.md", "网络流建模与结论.md"),
    ),
    Part(
        "06A - 数学（数论、组合与几何）.md",
        "数学：数论、组合、线性代数与几何",
        "06 - 数学",
        num(1, 7),
        ("README.md", "数学知识与结论.md"),
    ),
    Part(
        "06B - 数学（多项式、博弈与数值）.md",
        "数学：多项式、博弈、数值计算与位运算",
        "06 - 数学",
        num(8, 13),
    ),
    Part(
        "07 - 字符串.md",
        "字符串",
        "07 - 字符串",
        all_code,
        ("README.md", "字符串知识与结论.md"),
    ),
    Part(
        "08 - 高精度.md",
        "高精度",
        "08 - 高精度",
        all_code,
        ("README.md",),
    ),
    Part(
        "09 - 杂类.md",
        "杂类",
        "09 - 杂类",
        all_code,
        ("README.md", "杂类知识与结论.md", "09 - Python短代码.md"),
    ),
    Part(
        "10A - 动态规划（背包与状态设计）.md",
        "动态规划：背包与状态设计",
        "10 - 动态规划",
        num(1, 4),
        ("README.md", "动态规划知识与结论.md"),
    ),
    Part(
        "10B - 动态规划（优化与LCS）.md",
        "动态规划：优化与最长公共子序列",
        "10 - 动态规划",
        num(5, 6),
    ),
]


def natural(p: Path) -> list[object]:
    return [int(x) if x.isdigit() else x.casefold() for x in re.split(r"(\d+)", p.name)]


def demote(text: str, n: int) -> str:
    out: list[str] = []
    fenced = False
    for line in text.splitlines():
        if line.lstrip().startswith("```"):
            fenced = not fenced
            out.append(line)
        elif not fenced and re.match(r"^#{1,6}\s", line):
            m = re.match(r"^(#{1,6})(\s.*)$", line)
            assert m
            out.append("#" * min(6, len(m.group(1)) + n) + m.group(2))
        else:
            out.append(line)
    return "\n".join(out).rstrip() + "\n"


def rewrite_links(text: str, src: Path) -> str:
    """把源文档相对链接改成从打印稿目录可达的链接；代码围栏内不处理。"""
    out: list[str] = []
    fenced = False
    pat = re.compile(r"(\[[^\]]*\]\()([^)]+)(\))")
    for line in text.splitlines():
        if line.lstrip().startswith("```"):
            fenced = not fenced
            out.append(line)
            continue
        if fenced:
            out.append(line)
            continue

        def sub(m: re.Match[str]) -> str:
            raw = m.group(2).strip()
            target = raw.strip("<>")
            if not target or target.startswith(("http://", "https://", "mailto:", "#")):
                return m.group(0)
            base, mark, frag = target.partition("#")
            dst = (src.parent / unquote(base)).resolve()
            rel = os.path.relpath(dst, OUT).replace("\\", "/").replace(" ", "%20")
            if mark:
                rel += "#" + frag
            return m.group(1) + rel + m.group(3)

        out.append(pat.sub(sub, line))
    return "\n".join(out).rstrip() + "\n"


def sources(part: Part) -> list[Path]:
    folder = ROOT / part.folder
    return sorted(
        (
            p
            for p in folder.iterdir()
            if p.is_file()
            and p.suffix.lower() in part.exts
            and not p.name.startswith("_test")
            and part.pick(p)
        ),
        key=natural,
    )


COMMON = """## 公共粘贴环境

开局只保留完整版与极简版：完整版内联常用宏与函数，并在本地包含 `linyue.hpp`；极简版直接展开以下环境。除文件首部另有依赖说明外，C++ 片段默认接在其中一个版本后；多个备选实现定义同名类型时只选一个。

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define pii array<int, 2>
#define endl "\\n"
```

普通整数、下标、状态、权值和答案默认都使用宏展开后的 64 位 `int`。只有实际超时、超空间，或需要自然溢出、确定位宽和 128 位中间量时，才局部声明并使用 `i32 / i64 / u32 / u64 / i128 / u128`；固定宽度长类型名只出现在别名声明右侧。

开局头文件不预置短类型别名；`ld` 及上述整型别名均由实际使用它们的模板按需声明。代码不对 `.size()` 结果强转为 `int`；`size_t` 只保留在哈希返回值等标准库接口签名中。可能受 64 位下标影响的模板只在注释中提示卡常改法。

命名约定：模板/类型不超过 10 字符，函数不超过 7 字符，变量、字段、参数和模板参数不超过 3 字符；一般查询用 `ask`，定向查询用 `askMax / askMin / askGCD`，区间更新用 `update`，单点更新用 `modify`。同族模板统一类型和接口名。
"""


PRINT_CSS = r"""<style>
@page {
  size: A4;
  margin: 17mm 14mm 15mm;
  @top-center {
    content: "林月的 XCPC 算法模板";
    color: #555;
    font: 7.8pt "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
  }
  @bottom-center {
    content: counter(page);
    color: #555;
    font: 7.8pt "Noto Sans CJK SC", "Microsoft YaHei", sans-serif;
  }
}
@page cover {
  margin: 0;
  @top-center { content: none; }
  @bottom-center { content: none; }
}
html, body {
  margin: 0;
  padding: 0;
  color: #222;
  font-family: "Noto Sans CJK SC", "Microsoft YaHei", "SimSun", sans-serif;
  font-size: 9.4pt;
  line-height: 1.45;
}
a { color: #17468c; text-decoration: none; }
p { margin: 1.8mm 0; }
ul, ol { margin: 1.8mm 0; padding-left: 6mm; }
li { margin: .7mm 0; }
h1, h2, h3, h4 { color: #222; break-after: avoid-page; }
h1 { font-size: 18pt; }
h2 { margin: 5mm 0 2mm; font-size: 13.5pt; line-height: 1.3; }
h3 { margin: 4mm 0 1.5mm; font-size: 11.5pt; }
h4 { margin: 3mm 0 1mm; font-size: 10.2pt; }
table { width: 100%; border-collapse: collapse; font-size: 8.3pt; }
th, td { border: 0.2mm solid #bbb; padding: .9mm 1.4mm; vertical-align: top; }
th { background: #eee; font-weight: 700; }
blockquote { margin: 2mm 0; padding: .8mm 2.5mm; border-left: 1mm solid #999; color: #555; }
.katex {
  font-size: 1.03em;
}
.katex-display {
  margin: 2.2mm 0 2.8mm;
  text-align: center;
  break-inside: avoid;
}
.katex-display > .katex {
  max-width: 100%;
  font-size: 1em;
  text-align: center;
}
.katex-display > .katex > .katex-html {
  max-width: 100%;
}
.math-error {
  border: .2mm solid #a00;
  color: #a00;
}
code {
  font-family: "JetBrains Mono", "Cascadia Mono", "Consolas", monospace;
  font-size: .92em;
}
pre.code-block {
  margin: 2mm 0 3mm;
  padding: 1.2mm 0;
  border: .2mm solid #ddd;
  border-radius: .8mm;
  background: #f7f7f7;
  font-size: 7.15pt;
  line-height: 1.27;
  tab-size: 4;
  white-space: normal;
  break-inside: auto;
}
pre.code-block code { font-size: inherit; }
.code-line {
  display: grid;
  grid-template-columns: 7mm minmax(0, 1fr);
  min-height: 1.27em;
  break-inside: avoid;
}
.code-tail {
  display: block;
  break-inside: avoid-page;
}
.line-no {
  padding-right: 1.4mm;
  border-right: .2mm solid #ddd;
  color: #aaa;
  text-align: right;
  user-select: none;
}
.line-src {
  padding: 0 1.8mm;
  white-space: pre-wrap;
  overflow-wrap: anywhere;
}
.cover {
  page: cover;
  page-break-after: always;
  break-after: page;
  box-sizing: border-box;
  min-height: 297mm;
  padding: 34mm 25mm 22mm;
  color: #111;
  background: white;
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  text-align: center;
}
.cover-main { margin-top: 30mm; }
.cover-mark {
  width: 19mm;
  height: 19mm;
  margin: 0 auto 14mm;
  border: .7mm solid #111;
  display: grid;
  place-items: center;
  font: 700 15pt "Times New Roman", serif;
  letter-spacing: .08em;
}
.cover-rule { width: 84mm; height: .8mm; margin: 0 auto 10mm; background: #111; }
.cover h1 { margin: 0; color: #111; font: 700 31pt/1.2 "Noto Serif CJK SC", "SimSun", serif; border: 0; }
.cover-en { margin: 4mm 0 0; color: #333; font: 700 18pt/1.25 "Times New Roman", serif; }
.cover-desc { margin-top: 12mm; color: #555; font-size: 10pt; letter-spacing: .08em; }
.cover-meta { margin: 0; color: #222; font: 700 11pt/1.65 "Times New Roman", "SimSun", serif; }
.toc {
  page-break-after: always;
  break-after: page;
}
.toc > h1 {
  margin: 0 0 2mm;
  border: 0;
  text-align: center;
  font: 700 22pt "Noto Serif CJK SC", "SimSun", serif;
  letter-spacing: .22em;
}
.toc-meta { margin: 0 0 5mm; color: #666; text-align: center; font-size: 7.8pt; }
.toc-columns { column-count: 2; column-gap: 7mm; column-rule: .2mm solid #999; }
.toc-group { margin: 0 0 1.5mm; }
.toc-group h2 {
  display: flex;
  gap: 2mm;
  margin: 1mm 0 .5mm;
  padding: .35mm 1mm;
  border: 0;
  background: #e1e1e1;
  color: #111;
  font-size: 8pt;
  line-height: 1.2;
}
.toc-group h2 a { color: #111; }
.toc-group-page { margin-left: auto; }
.toc-entry {
  display: flex;
  align-items: flex-end;
  gap: .8mm;
  margin: .25mm 0;
  font-size: 7.4pt;
  line-height: 1.22;
  break-inside: avoid;
}
.toc-entry a { min-width: 0; color: #222; }
.toc-dots { flex: 1 1 5mm; min-width: 3mm; margin-bottom: .75mm; border-bottom: .2mm dotted #777; }
.toc-page { flex: none; color: #222; font-family: "Cascadia Mono", "Consolas", monospace; font-variant-numeric: tabular-nums; }
.part-cover {
  page-break-before: always;
  break-before: page;
  margin: 0 0 5mm;
  padding: 0 0 3mm;
  border-bottom: .6mm solid #333;
}
.part-no { color: #666; font: 700 8pt "Cascadia Mono", monospace; letter-spacing: .1em; }
.part-cover h1 { string-set: chapter content(); margin: 2mm 0 1mm; font-size: 18pt; border: 0; }
.part-meta { margin: 0; color: #666; font-size: 7.8pt; }
.template-title { margin-top: 5mm; font-size: 13.5pt; border: 0; }
.template-title code, .doc-title code { color: #222; font: inherit; }
.doc-title { margin-top: 5mm; font-size: 13pt; }
.page-key { color: white; font: 1pt/0 "Arial", sans-serif; }
.toc-detail { table-layout: fixed; }
.toc-detail th:first-child, .toc-detail td:first-child { width: 13mm; }
.toc-detail th:last-child, .toc-detail td:last-child { width: 12mm; text-align: right; white-space: nowrap; }
tr { break-inside: avoid; }
.page-break { display: none; }
</style>"""


def part_id(part: Part) -> str:
    m = re.match(r"^([0-9A-Z]+)", part.out)
    assert m
    return "part-" + m.group(1).lower()


def template_id(part: Part, i: int) -> str:
    return f"{part_id(part)}-template-{i:03d}"


def doc_id(part: Part, i: int) -> str:
    return f"{part_id(part)}-doc-{i:03d}"


def page_ref(key: str) -> str:
    return f'<span class="toc-page">{PAGE_MAP.get(key, 0)}</span>'


def page_marker(key: str) -> str:
    return f'<span class="page-key">LYPAGE:{key}:END</span>'


def part_cover(part: Part, count: int) -> str:
    key = re.match(r"^([0-9A-Z]+)", part.out)
    assert key
    return "\n".join(
        [
            f'<section class="part-cover" id="{part_id(part)}">',
            f'  <div class="part-no">PART {key.group(1)}{page_marker(part_id(part))}</div>',
            f'  <h1>{html.escape(part.title)}</h1>',
            f'  <p class="part-meta">{count} 个代码模板 · {OWNER} · {VERSION} · {UPDATED}</p>',
            "</section>",
        ]
    )


def render(part: Part, src: list[Path], styled: bool = True, integrated: bool = False) -> str:
    lines: list[str] = []
    if styled:
        lines.extend(["<!-- 本文件由生成打印稿.py 生成，请勿直接编辑正文。 -->", PRINT_CSS, ""])
    lines.extend([part_cover(part, len(src)), ""])
    if not integrated:
        lines.extend([COMMON.rstrip(), ""])
        lines.extend(["## 本册目录", "", '<table class="toc-detail">', '<thead><tr><th>类型</th><th>条目</th><th>页码</th></tr></thead>', '<tbody>'])
        for i, name in enumerate(part.docs, 1):
            key = doc_id(part, i)
            lines.append(f'<tr><td>知识</td><td><a href="#{key}"><code>{html.escape(name)}</code></a></td><td>{page_ref(key)}</td></tr>')
        for i, p in enumerate(src, 1):
            key = template_id(part, i)
            lines.append(f'<tr><td>代码</td><td><a href="#{key}"><code>{html.escape(p.name)}</code></a></td><td>{page_ref(key)}</td></tr>')
        lines.extend(['</tbody>', '</table>', ""])

    if part.docs:
        lines.extend(["## 版块说明与知识结论", ""])
        for i, name in enumerate(part.docs, 1):
            p = ROOT / part.folder / name
            if not p.exists():
                raise FileNotFoundError(p)
            doc = rewrite_links(p.read_text(encoding="utf-8"), p)
            key = doc_id(part, i)
            lines.extend(
                [
                    f'<h2 id="{key}" class="doc-title"><code>{html.escape(name)}</code>{page_marker(key)}</h2>',
                    "",
                ]
            )
            lines.append(demote(doc, 2).rstrip())
            lines.extend(["", '<div class="page-break"></div>', ""])

    for i, p in enumerate(src, 1):
        lang = "cpp" if p.suffix.lower() in {".cpp", ".hpp"} else "text"
        code = p.read_text(encoding="utf-8").rstrip()
        lines.extend(
            [
                f'<h2 id="{template_id(part, i)}" class="template-title"><code>{html.escape(p.name)}</code>{page_marker(template_id(part, i))}</h2>',
                "",
                f"```{lang}",
                code,
                "```",
                "",
            ]
        )
    return "\n".join(lines).rstrip() + "\n"


def validate_assignment(rendered: dict[str, list[Path]]) -> None:
    by_folder: dict[str, list[Part]] = {}
    for p in PARTS:
        by_folder.setdefault(p.folder, []).append(p)
    for folder, parts in by_folder.items():
        exts = {x for p in parts for x in p.exts}
        expected = {
            p.resolve()
            for p in (ROOT / folder).iterdir()
            if p.is_file() and p.suffix.lower() in exts and not p.name.startswith("_test")
        }
        got = [p.resolve() for part in parts for p in rendered[part.out]]
        if len(got) != len(set(got)):
            raise RuntimeError(f"分册重复收录：{folder}")
        missing = expected - set(got)
        extra = set(got) - expected
        if missing or extra:
            raise RuntimeError(f"分册覆盖异常：{folder}; missing={missing}; extra={extra}")


def validate_numbering() -> None:
    folders = sorted({p.folder for p in PARTS}, key=lambda x: int(x[:2]))
    got = [int(p[:2]) for p in folders]
    if got != list(range(1, 11)):
        raise RuntimeError(f"顶层编号不是唯一连续的 01..10：{folders}")
    for folder in folders:
        seen: dict[str, str] = {}
        for p in (ROOT / folder).iterdir():
            if not p.is_file() or p.suffix.lower() not in {".cpp", ".hpp"} or p.name == "linyue.hpp":
                continue
            m = re.match(r"^(\d{2}[A-Z]*) - ", p.name)
            if not m:
                raise RuntimeError(f"源码缺少规范编号：{p.relative_to(ROOT)}")
            key = m.group(1)
            if key in seen:
                raise RuntimeError(f"同目录重复索引：{folder}/{seen[key]} 与 {p.name}")
            seen[key] = p.name


def validate_fences(text: str, name: str) -> None:
    if sum(1 for line in text.splitlines() if line.lstrip().startswith("```")) % 2:
        raise RuntimeError(f"代码围栏未闭合：{name}")


def validate_links(text: str, src: Path) -> None:
    pat = re.compile(r"(?<!!)\[[^\]]*\]\(([^)]+)\)")
    fenced = False
    for line in text.splitlines():
        if line.lstrip().startswith("```"):
            fenced = not fenced
            continue
        if fenced:
            continue
        for raw in pat.findall(line):
            target = raw.strip().strip("<>")
            if not target or target.startswith(("#", "http://", "https://", "mailto:")):
                continue
            base = target.partition("#")[0]
            dst = (src.parent / unquote(base)).resolve()
            if not dst.exists():
                raise RuntimeError(f"失效本地链接：{src.name} -> {target}")


def validate_anchors(text: str, name: str) -> None:
    ids = re.findall(r'\bid="([^"]+)"', text)
    if len(ids) != len(set(ids)):
        raise RuntimeError(f"HTML 锚点重复：{name}")
    targets = [x for x in re.findall(r"\]\(#([^)]+)\)", text) if x.startswith("part-")]
    missing = sorted(set(targets) - set(ids))
    if missing:
        raise RuntimeError(f"目录锚点失效：{name}; missing={missing}")


def validate_project_markdown() -> None:
    for p in ROOT.rglob("*.md"):
        rel = p.relative_to(ROOT)
        if "node_modules" in rel.parts or OUT / "参考资料归档" in p.parents:
            continue
        text = p.read_text(encoding="utf-8")
        validate_fences(text, str(p.relative_to(ROOT)))
        validate_links(text, p)


def cover(total: int, sections: int) -> str:
    return "\n".join(
        [
            '<section class="cover">',
            '  <div class="cover-main">',
            '    <div class="cover-mark">LY</div>',
            '    <div class="cover-rule"></div>',
            '    <h1>林月的 XCPC 算法模板</h1>',
            '    <p class="cover-en">XCPC Algorithm Template</p>',
            '    <p class="cover-desc">代码模板 · 知识结论 · 赛时页码索引</p>',
            '  </div>',
            f'  <p class="cover-meta">{VERSION}　{UPDATED}<br>{OWNER}</p>',
            '</section>',
        ]
    )


def contents(rendered: dict[str, list[Path]]) -> str:
    lines = [
        '<section class="toc" id="contents">',
        '  <h1>目录</h1>',
        f'  <p class="toc-meta">{OWNER} · {VERSION} · {UPDATED} · 页码与纸面物理页一致</p>',
        '  <div class="toc-columns">',
    ]
    for part in PARTS:
        lines.extend(
            [
                '    <section class="toc-group">',
                f'      <h2><a href="#{part_id(part)}">{html.escape(part.title)}</a><span class="toc-group-page">{page_ref(part_id(part))}</span></h2>',
            ]
        )
        for i, name in enumerate(part.docs, 1):
            key = doc_id(part, i)
            title = Path(name).stem
            lines.append(f'      <div class="toc-entry"><a href="#{key}">{html.escape(title)}</a><span class="toc-dots"></span>{page_ref(key)}</div>')
        for i, p in enumerate(rendered[part.out], 1):
            key = template_id(part, i)
            lines.append(f'      <div class="toc-entry"><a href="#{key}">{html.escape(p.stem)}</a><span class="toc-dots"></span>{page_ref(key)}</div>')
        lines.append('    </section>')
    lines.extend(
        [
            '  </div>',
            '</section>',
        ]
    )
    return "\n".join(lines)


def meta_fields(p: Path) -> dict[str, str]:
    keys = {"用途", "复杂度", "性能", "依赖", "限制", "接口", "用法", "约定"}
    got: dict[str, str] = {}
    for raw in p.read_text(encoding="utf-8").splitlines():
        line = raw.strip().lstrip("*").strip()
        m = re.match(r"^([^：]+)：(.*)$", line)
        if m and m.group(1) in keys:
            got.setdefault(m.group(1), m.group(2).strip().rstrip("*/").strip())
    return got


def md_cell(text: str) -> str:
    text = re.sub(r"\s+", " ", text.strip())
    return html.escape(text, quote=False).replace("|", r"\|")


def local_url(p: Path) -> str:
    rel = os.path.relpath(p, ROOT).replace("\\", "/")
    return quote(rel, safe="/")


def pdf_page(page: int | None) -> str:
    if not page:
        return "—"
    if not FINAL_PDF.is_file():
        return str(page)
    return f"[{page}]({local_url(FINAL_PDF)}#page={page})"


def local_index(rendered: dict[str, list[Path]]) -> str:
    by_folder: dict[str, list[Part]] = {}
    for part in PARTS:
        by_folder.setdefault(part.folder, []).append(part)
    folders = sorted(by_folder, key=lambda x: int(x[:2]))
    total = sum(len(v) for v in rendered.values())
    docs = sum(len(p.docs) for p in PARTS)
    lines = [
        "<!-- 本文件由 00 - 打印稿整理/生成打印稿.py 生成，请勿直接编辑。 -->",
        "# 林月的 XCPC 本地全模板索引",
        "",
        f"> 所有者：{OWNER}；版本：{VERSION}；更新时间：{UPDATED}；共 10 个版块、{len(PARTS)} 个分册、{total} 份代码模板、{docs} 个知识入口。",
        "",
        "本索引面向本地赛时检索。使用 `Ctrl+F` 搜索中文名称、英文别名、编号、用途或环境词；点击模板名直接打开源码，点击 PDF 页码跳转到最终打印稿的物理页。",
        "",
        "建议环境词：`极致优化`、`常规`、`GNU环境`、`小数据环境`、`自动取模`、`with Int`、`Python`；常用别名可直接搜索 `DSU / BIT / LCT / PBDS / HLPP / NTT / FFT / CRTNTT / KMP / PAM / SAM / SA / SG`。",
        "",
        "## 总目录",
        "",
        "|编号|版块|代码模板|知识入口|本地目录|",
        "|---:|---|---:|---:|---|",
    ]
    for folder in folders:
        parts = by_folder[folder]
        num_ = folder[:2]
        title = folder.split(" - ", 1)[1]
        count = sum(len(rendered[p.out]) for p in parts)
        doc_count = sum(len(p.docs) for p in parts)
        url = quote(folder + "/", safe="/")
        lines.append(f'|{num_}|[{md_cell(title)}](#part-{num_})|{count}|{doc_count}|[打开目录]({url})|')

    fallback = {
        "01A - 头文件（完整版）": "完整开局环境；本地接入 linyue.hpp，提交时自动关闭调试输出。",
        "01B - 头文件（极简版）": "只保留竞赛必需头文件、宏、solve 与 main。",
        "linyue": "本地调试输出辅助文件，仅由完整版头文件在非 ONLINE_JUDGE 环境包含。",
    }
    for folder in folders:
        parts = by_folder[folder]
        num_ = folder[:2]
        title = folder.split(" - ", 1)[1]
        lines.extend(
            [
                "",
                f'<a id="part-{num_}"></a>',
                f"## {num_} - {title}",
                "",
                f"本地目录：[`{folder}/`]({quote(folder + '/', safe='/')})",
            ]
        )
        part_docs: list[tuple[Part, int, str]] = []
        for part in parts:
            part_docs.extend((part, i, name) for i, name in enumerate(part.docs, 1))
        if part_docs:
            lines.extend(["", "### 知识与选型入口", "", "|分册|文档|PDF 页|", "|---|---|---:|"])
            for part, i, name in part_docs:
                p = ROOT / part.folder / name
                page = PAGE_MAP.get(doc_id(part, i))
                lines.append(f"|{md_cell(part.title)}|[{md_cell(name)}]({local_url(p)})|{pdf_page(page)}|")

        for part in parts:
            if len(parts) > 1:
                lines.extend(["", f"### {part.title}"])
            lines.extend(
                [
                    "",
                    "|编号|模板（点击打开本地源码）|用途|复杂度 / 性能提示|依赖|PDF 页|",
                    "|---|---|---|---|---|---:|",
                ]
            )
            for i, p in enumerate(rendered[part.out], 1):
                m = re.match(r"^(\d{2}[A-Z]*) - ", p.name)
                idx = m.group(1) if m else "—"
                meta = meta_fields(p)
                purpose = meta.get("用途") or fallback.get(p.stem) or re.sub(r"^\d{2}[A-Z]* - ", "", p.stem)
                cost = meta.get("复杂度") or meta.get("性能") or meta.get("限制") or "—"
                dep = meta.get("依赖") or "—"
                page = PAGE_MAP.get(template_id(part, i))
                name = md_cell(p.name)
                lines.append(
                    f"|{idx}|[{name}]({local_url(p)})|{md_cell(purpose)}|{md_cell(cost)}|{md_cell(dep)}|{pdf_page(page)}|"
                )

    lines.extend(
        [
            "",
            "## 更新方式",
            "",
            "本文件与分册打印稿共用同一份收录规则。新增、删除或重命名模板后，在项目根目录运行：",
            "",
            "```powershell",
            "python '.\\00 - 打印稿整理\\生成打印稿.py'",
            "```",
            "",
            "生成器会同时检查顶层编号、目录内索引、重复或遗漏收录、Markdown 围栏及本地链接。PDF 页码发生变化时，再运行 `生成最终PDF.py` 回填新页码。",
        ]
    )
    return "\n".join(lines).rstrip() + "\n"


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    texts: dict[str, str] = {}
    rendered: dict[str, list[Path]] = {}
    for part in PARTS:
        src = sources(part)
        rendered[part.out] = src

    validate_numbering()
    validate_assignment(rendered)
    for part in PARTS:
        text = render(part, rendered[part.out])
        validate_fences(text, part.out)
        (OUT / part.out).write_text(text, encoding="utf-8", newline="\n")
        texts[part.out] = text

    total = sum(len(v) for v in rendered.values())
    full = [
        "<!-- 本文件由生成打印稿.py 生成，请勿直接编辑正文。 -->",
        PRINT_CSS,
        "",
        cover(total, len({p.folder for p in PARTS})),
        "",
        contents(rendered),
        "",
    ]
    for part in PARTS:
        full.extend([render(part, rendered[part.out], styled=False, integrated=True).rstrip(), ""])
    full_text = "\n".join(full).rstrip() + "\n"
    validate_fences(full_text, "整合稿")
    validate_anchors(full_text, "整合稿")
    integrated = "00 - XCPC全量打印稿.md"
    (OUT / integrated).write_text(full_text, encoding="utf-8", newline="\n")

    index_text = local_index(rendered)
    LOCAL_INDEX.write_text(index_text, encoding="utf-8", newline="\n")

    legacy = OUT / "LYE-XCPC.md"
    if legacy.exists():
        legacy.unlink()

    readme = [
        "# XCPC 打印稿",
        "",
        f"> 所有者：{OWNER}；版本：{VERSION}；更新时间：{UPDATED}；共 {len(PARTS)} 个分册、{total} 个代码文件。",
        "",
        "- [最终 PDF：林月的 XCPC 算法模板 v1.0.0](../output/pdf/林月的XCPC算法模板-v1.0.0.pdf)",
        "- [本地全模板索引](../模板索引.md)",
        "- [纸面页码索引](页码索引.md)",
        f"- [全量整合打印稿]({integrated.replace(' ', '%20')})",
        "- [参考资料严格包含与覆盖证明](参考资料对照.md)",
        "- [卡常模板与《板子.md》复核报告](卡常模板与板子复核报告.md)",
        "- [全项目复查报告](项目复查报告.md)",
        "",
        "## 分册",
        "",
    ]
    readme.extend(f"- [{p.title}]({p.out.replace(' ', '%20')})" for p in PARTS)
    readme.extend(
        [
            "",
            "## 重新生成",
            "",
            "```powershell",
            "python '.\\00 - 打印稿整理\\生成打印稿.py'",
            "python '.\\00 - 打印稿整理\\生成最终PDF.py'",
            "```",
            "",
            "生成器会验证顶层编号唯一、目录内索引不重复、每个源码恰好进入一个分册，并检查 Markdown 代码围栏与本地链接。`生成最终PDF.py` 首次运行会按 `package-lock.json` 安装本地渲染依赖。",
            "",
            "## 打印",
            "",
            f"最终稿固定署名 {OWNER}，版本 {VERSION}，更新时间 {UPDATED}。`生成最终PDF.py` 使用 KaTeX 静态渲染行内与行间公式；公式无法解析或分隔符残留时会停止生成。随后生成器会迭代排版并回填物理页码，输出到 `output/pdf/`；打印时关闭浏览器自带页眉页脚。",
        ]
    )
    readme_text = "\n".join(readme) + "\n"
    (OUT / "README.md").write_text(readme_text, encoding="utf-8", newline="\n")
    for name, text in {**texts, integrated: full_text, "README.md": readme_text}.items():
        validate_links(text, OUT / name)
        validate_anchors(text, name)
    validate_project_markdown()
    print(f"generated {len(PARTS)} parts, {total} code files, integrated={integrated}")


if __name__ == "__main__":
    main()
