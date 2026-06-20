# MiniBrowser (Stack Edition)

Two small, self-contained demonstrations of how browsers actually use
**stacks** internally — built with nothing beyond arrays, strings, and
`std::stack`.

## 1. HTML Tag Validator (`include/html_validator.hpp`)

Checks that HTML tags are properly opened, closed, and nested — the exact
same problem as validating matched brackets `( [ { } ] )`, just with
`<tag>` / `</tag>` instead of brackets.

**How it works:** scan the string left to right. Every opening tag gets
pushed onto a stack. Every closing tag must match whatever's on top of the
stack — if it doesn't, or the stack is empty, the document is invalid.
Self-closing tags (`<br/>`) and void elements (`<img>`, `<hr>`, etc.) are
never pushed, since they never get a closing tag.

This is genuinely the first check a real HTML parser performs before it
can build anything — Chromium's HTML parser does a much more elaborate
version of this same idea.

## 2. Browser History (`include/browser_history.hpp`)

Models back/forward navigation with **two stacks** — `backStack` and
`forwardStack` — plus a pointer to the current page. This isn't an
analogy; it's literally the standard way browsers implement history:

- `visit(url)` → push current page onto `backStack`, go to the new page,
  and clear `forwardStack` (a new navigation invalidates old forward history)
- `back()` → push current page onto `forwardStack`, pop from `backStack`
- `forward()` → push current page onto `backStack`, pop from `forwardStack`

## Build & run

```bash
g++ -std=c++17 -Iinclude src/main.cpp -o minibrowser -Wall -Wextra
./minibrowser
```

The program first runs the validator against five test cases (valid
nesting, mismatched tags, unclosed tags, stray closing tags), then drops
into an interactive REPL where you can type `visit <url>`, `back`,
`forward`, `current`, `exit`.
