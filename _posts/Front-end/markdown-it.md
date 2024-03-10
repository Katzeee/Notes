#software-design #productivity/markdown #nodejs

## 2023.08.16

> https://github.com/markdown-it/markdown-it

## `render` and `renderInline`

### `render`

It takes the whole markdown document as a string despite of the input text.

```javascript
var md = require('markdown-it')();
var result = md.render('# markdown-it rulezz!');
console.log(result); // Outputs: <h1>markdown-it rulezz!</h1>
```

### `renderInline`

It also takes a string as input, but it treats this string as a single standalone line of text without any block-level tags.

Only processes inline markdown syntax such as strong or emphasis (i.e., **bold** or _italic_) and **does not** process block-level elements like headings, lists, or code blocks.

```javascript
var md = require('markdown-it')();
var result = md.renderInline('__markdown-it__ rulezz!');
console.log(result); // Outputs: <strong>markdown-it</strong> rulezz!
```

!!! Summary Summary

    `render` processes both block-level and inline-level markdown elements while `renderInline` only processes inline-level markdown elements.

## Ruler

### Default inline ruler

- `[Function: text]`: Render plain text.
- `[Function: linkify]`: Convert URLs and email addresses into clickable links.
- `[Function: newline]`: Handle line breaks and render them as `<br>` tags.
- `[Function: escape]`: Escape special characters.
- `[Function: backtick]`: Deals with inline code and render them as `<code>` tags.
- `[Function: strikethrough]`: Detect double tildes (`~~`).
- `[Function: emphasis]`: Identifies single asterisks or underscores and renders them as `<em>` tags (italicized).
- `[Function: link]`: Parse and renders links into `<a>` tags.
- `[Function: image]`: Handle the images into `<img>` tags.
- `[Function: autolink]`: Automatically detects and converts plain URLs into clickable links.
- `[Function: html_inline]`: Handle raw HTML code.
- `[Function: entity]`: Convert HTML entities (e.g., `&amp;`) into their respective characters.

!!! Hint The return value of rule function

    A rule process function returns `true` means the input string is exactly that kind of format.
    The render process will continue to next procedure skipping all left rules when `parse_block` and `parse_inline`.

## State

### Token

Every token is node of AST while parse the whole markdown file.

```javascript
function Token(type, tag, nesting) {
	// Type of the token (string, e.g. "paragraph_open")
	this.type = type;
	// html tag name, e.g. "p"
	this.tag = tag;
	// Html attributes. Format: `[ [ name1, value1 ], [ name2, value2 ] ]`
	this.attrs = null;
  // Source map info. Format: `[ line_begin, line_end ]`
	this.map = null;
	/**
	 * Token#nesting -> Number
	 *
	 * Level change (number in {-1, 0, 1} set), where:
	 *
	 * -  `1` means the tag is opening
	 * -  `0` means the tag is self-closing
	 * - `-1` means the tag is closing
	 **/
	this.nesting = nesting;
  // nesting level, the same as `state.level`
	this.level = 0;
  // An array of child nodes (inline and img tokens)
	this.children = null;
  // In a case of self-closing tag (code, html, fence, etc.), it has contents of this tag.
	this.content = '';
  // '*' or '_' for emphasis, fence string for fence, etc.
	this.markup = '';
	/**
	 * Token#info -> String
	 *
	 * Additional information:
	 *
	 * - Info string for "fence" tokens
	 * - The value "auto" for autolink "link_open" and "link_close" tokens
	 * - The string value of the item marker for ordered-list "list_item_open" tokens
	 **/
	this.info = '';
  // A place for plugins to store an arbitrary data
	this.meta = null;
	// True for block-level tokens, false for inline tokens. Used in renderer to calculate line breaks
	this.block = false;
	// If it's true, ignore this element when rendering. Used for tight lists to hide paragraphs.
	this.hidden = false;
}
```

`state.push` will generate a new node and add it to the AST.

```javascript
// rules_block/state_block.js
StateBlock.prototype.push = function (type, tag, nesting) {
	var token = new Token(type, tag, nesting);
	token.block = true;

	if (nesting < 0) this.level--; // closing tag
	token.level = this.level;
	if (nesting > 0) this.level++; // opening tag

	this.tokens.push(token);
	return token;
};
```

### `state.option.typographer`

It is an option while rendering, choose whether to apply some typography replacement rule, like:

- Quotes replacement (e.g. straight quotes to curly quotes)
- Dashes replacement (hyphens to ndash/mdash)
- Ellipsis formatting
- and more...

## `Render` call chain

```javascript
MarkdownIt.prototype.render = function (src, env);
MarkdownIt.prototype.parse = function (src, env);
function core.StateCore(src, md, env);
this.core.process(state);
rules = this.ruler.getRules('');
rules[i](state); // for every rule
  function block(state); // rule block
    ParserBlock.prototype.parse = function (src, md, env, outTokens);
    ParserBlock.prototype.tokenize = function (state, startLine, endLine); // get all rules and apply them, create a token AST
  function inline(state); // rule inline, parses every block token whose type is inline
		ParserInline.prototype.parse = function (str, md, env, outTokens);
		ParserInline.prototype.tokenize = function (state); // get all rules and apply them, create a token AST
		rules = this.ruler2.getRules(''); // apply rule2
Renderer.prototype.render = function (tokens, options, env);
```

`Renderer.prototype.render` are responsible for outputting the final result of the render(i.e. the html file). It takes the AST tokens for final render.

!!! Hint Hint

    `render` parse input md string to tokens by calling `MarkdownIt.prototype.parse = function (src, env);`(line 2) while `renderInline` calls `MarkdownIt.prototype.parseInline = function (src, env);`

```javascript
Renderer.prototype.render = function (tokens, options, env) {
	var i,
		len,
		type,
		result = '',
		rules = this.rules;

	for (i = 0, len = tokens.length; i < len; i++) {
		type = tokens[i].type;

		if (type === 'inline') {
			result += this.renderInline(tokens[i].children, options, env);
		} else if (typeof rules[type] !== 'undefined') {
			result += rules[tokens[i].type](tokens, i, options, env, this);
		} else {
			result += this.renderToken(tokens, i, options, env);
		}
	}

	return result;
};
```
