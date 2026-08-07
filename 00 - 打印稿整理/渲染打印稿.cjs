const fs = require("fs");
const path = require("path");
const { pathToFileURL } = require("url");
const katex = require("katex");
const { marked } = require("marked");

if (process.argv.length !== 4) {
  console.error("usage: node 渲染打印稿.cjs <input.md> <output.html>");
  process.exit(2);
}

const input = path.resolve(process.argv[2]);
const output = path.resolve(process.argv[3]);
const source = fs.readFileSync(input, "utf8");

const esc = (s) => s
  .replaceAll("&", "&amp;")
  .replaceAll("<", "&lt;")
  .replaceAll(">", "&gt;")
  .replaceAll('"', "&quot;");

let inlineMath = 0;
let displayMath = 0;
const mathErrors = [];

function renderMath(text, displayMode) {
  const tex = text.trim();
  try {
    const result = katex.renderToString(tex, {
      displayMode,
      output: "htmlAndMathml",
      strict: "ignore",
      throwOnError: true,
      trust: false,
    });
    if (displayMode) ++displayMath;
    else ++inlineMath;
    return result;
  } catch (error) {
    mathErrors.push(`${displayMode ? "行间" : "行内"}公式：${tex}\n${error.message}`);
    return `<span class="math-error">${esc(tex)}</span>`;
  }
}

const blockMath = {
  name: "blockMath",
  level: "block",
  start(src) {
    const dollar = src.indexOf("$$");
    const bracket = src.indexOf("\\[");
    if (dollar < 0) return bracket;
    if (bracket < 0) return dollar;
    return Math.min(dollar, bracket);
  },
  tokenizer(src) {
    const dollar = /^\$\$[ \t]*(?:\r?\n)?([\s\S]+?)(?:\r?\n)?[ \t]*\$\$(?:[ \t]*(?:\r?\n|$))/.exec(src);
    if (dollar) return { type: "blockMath", raw: dollar[0], text: dollar[1] };
    const bracket = /^\\\[[ \t]*(?:\r?\n)?([\s\S]+?)(?:\r?\n)?[ \t]*\\\](?:[ \t]*(?:\r?\n|$))/.exec(src);
    if (bracket) return { type: "blockMath", raw: bracket[0], text: bracket[1] };
  },
  renderer(token) {
    return `${renderMath(token.text, true)}\n`;
  },
};

const spanMath = {
  name: "spanMath",
  level: "inline",
  start(src) {
    const dollar = src.indexOf("$");
    const paren = src.indexOf("\\(");
    if (dollar < 0) return paren;
    if (paren < 0) return dollar;
    return Math.min(dollar, paren);
  },
  tokenizer(src) {
    const dollar = /^\$(?!\$)((?:\\.|[^\\$\r\n])+?)\$(?!\$)/.exec(src);
    if (dollar) return { type: "spanMath", raw: dollar[0], text: dollar[1] };
    const paren = /^\\\(((?:\\.|[^\\\r\n])+?)\\\)/.exec(src);
    if (paren) return { type: "spanMath", raw: paren[0], text: paren[1] };
  },
  renderer(token) {
    return renderMath(token.text, false);
  },
};

marked.use({
  gfm: true,
  breaks: false,
  extensions: [blockMath, spanMath],
  renderer: {
    code({ text, lang }) {
      const cls = lang ? ` class="language-${esc(lang.split(/\s+/)[0])}"` : "";
      const rows = text.replace(/\n$/, "").split("\n").map((line, i) =>
        `<span class="code-line"><span class="line-no">${i + 1}</span><span class="line-src">${line ? esc(line) : "&#8203;"}</span></span>`
      );
      const cut = Math.max(0, rows.length - 12);
      const body = rows.slice(0, cut).join("") + `<span class="code-tail">${rows.slice(cut).join("")}</span>`;
      return `<pre class="code-block"><code${cls}>${body}</code></pre>\n`;
    },
  },
});

const body = marked.parse(source);
if (mathErrors.length) {
  console.error(`KaTeX 无法解析 ${mathErrors.length} 个公式：`);
  console.error(mathErrors.map((x, i) => `\n[${i + 1}] ${x}`).join("\n"));
  process.exit(1);
}

// 公式分隔符若仍残留在普通正文中，说明 Markdown 扩展没有消费它；代码块和
// 行内代码中的 $ 属于源码内容，不参与此检查。
const prose = body
  .replace(/<pre\b[\s\S]*?<\/pre>/gi, "")
  .replace(/<code\b[\s\S]*?<\/code>/gi, "")
  .replace(/<style\b[\s\S]*?<\/style>/gi, "");
const unresolved = prose.match(/\$\$|\$(?!\s)(?:\\.|[^$\r\n])+?\$/g) || [];
if (unresolved.length) {
  console.error(`仍有 ${unresolved.length} 个未渲染的公式分隔符：`);
  console.error(unresolved.slice(0, 10).join("\n"));
  process.exit(1);
}

if (inlineMath + displayMath === 0 && /\$\$|\$(?!\s)[^$\r\n]+\$/.test(source)) {
  console.error("Markdown 含公式，但 KaTeX 渲染计数为 0");
  process.exit(1);
}

const katexCss = pathToFileURL(require.resolve("katex/dist/katex.min.css")).href;
const document = `<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="author" content="林月">
  <meta name="version" content="v1.0.0">
  <meta name="updated" content="2026.8.1">
  <meta name="math-renderer" content="KaTeX ${katex.version}">
  <meta name="math-count" content="${inlineMath + displayMath}">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>林月的 XCPC 算法模板 v1.0.0</title>
  <link rel="stylesheet" href="${katexCss}">
</head>
<body>
${body}
</body>
</html>
`;

fs.mkdirSync(path.dirname(output), { recursive: true });
fs.writeFileSync(output, document, "utf8");
console.log(`html=${output}; math=${inlineMath + displayMath}; inline=${inlineMath}; display=${displayMath}; katex=${katex.version}`);
