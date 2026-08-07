from __future__ import annotations

import re
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
PRINT = ROOT / "00 - 打印稿整理"
SOURCE = PRINT / "参考资料归档" / "原始文件" / "WIDA-XCPC-v1.8.8-2024.11.19.md"
OUT = PRINT / "参考标题覆盖清单.md"

DEST = {
    "基础算法": "01 - 基础算法",
    "树上问题": "02 - 树上问题",
    "图论": "04 - 图论",
    "网络流": "05 - 网络流",
    "数论": "06 - 数学 / 数论、组合",
    "几何": "06 - 数学 / 几何",
    "多项式": "06 - 数学 / 多项式",
    "数据结构": "03 - 数据结构",
    "动态规划": "10 - 动态规划",
    "串": "07 - 字符串",
    "博弈论": "06 - 数学 / 博弈论",
    "STL": "01 - 基础算法 / STL 速查",
    "卡常": "01 - 基础算法及对应算法环境分支",
    "杂类": "09 - 杂类（可归类项已进入对应版块）",
}


def main() -> None:
    rows = []
    section = ""
    fenced = False
    for no, line in enumerate(SOURCE.read_text(encoding="utf-8-sig").splitlines(), 1):
        if line.startswith("```"):
            fenced = not fenced
            continue
        if fenced:
            continue
        m = re.match(r"^(#{2,4})\s+(.+?)\s*$", line)
        if not m:
            continue
        level, title = len(m.group(1)), m.group(2)
        if level == 2:
            section = title
        dest = DEST[section]
        title = title.replace("|", "\\|")
        rows.append((no, level, section, title, dest))

    lines = [
        "# 参考标题逐项覆盖清单",
        "",
        "> 原 Markdown 的每一个非代码标题均列在此处；原文在归档中逐字节保存，主库入口按林月版分类给出。",
        "",
        f"共 **{len(rows)}** 个标题，其中一级版块 **{sum(x[1] == 2 for x in rows)}** 个。",
        "",
        "|原行|层级|原版块|原标题|林月版主库入口|覆盖方式|",
        "|---:|---:|---|---|---|---|",
    ]
    for no, level, section, title, dest in rows:
        lines.append(f"|{no}|H{level}|{section}|{title}|`{dest}`|主库整理 + 原文精确归档|")
    OUT.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    print(f"generated {OUT}; headings={len(rows)}")


if __name__ == "__main__":
    main()

