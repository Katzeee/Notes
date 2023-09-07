#productivity/markdown
## 2023.08.15

## Basic

- hard line break(two or more spaces at the end of line)

	123  
	123

- _underline_

- ==mark==

- ~sub~script

- ^super^script

- [ ] checkbox

## Advanced Syntax

- admonition

  !!! note "Note

		This is a note

	!!! danger 

		This is danger
      
- TOC

	[[TOC]]

- footnote

	You can search here[^1].

	[^1]: [[google.com]]

- abbr

	TLDR

	*[TLDR]: Too long, don't read.

- deflist

	Word

	: First defination
	: Second defination

- attrs

	**hi**{style="color:red;background-color:white;"}

- kbd

	[[ctrl + escape]] <kbd>ctrl + escape</kbd>

- emoji

	:smile:
	 
- container

	::::: container
	:::: row
	::: col-xs-6 alert alert-success
	success text
	:::
	::: col-xs-6 alert alert-warning
	warning text
	:::
	::::
	:::::

::: warning
*here be dragons*
:::
