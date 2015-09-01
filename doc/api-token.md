# Token

Describes a parsed token in a file. Each ASTNode contains a set of associated tokens. For example, the following c++ class, will have tokens `['class', 'Date', '{', '}']` associated with it.

```
class Date{
};
```

The token list can be retrieved by the following method:

```js
var firstClassNode = codeBase.find('Date', 'class')[0];
var associatedTokens = firstClassNode.associatedTokens();
```

## INDEX

 - [ENUMS](#enums)
	 - [TokenKind](#TokenKind)
 - [METHODS](#methods)
	 - [associatedNode](#associatedNode)
	 - [name](#column)
	 - [offset](#offset)
	 - [filePath](#filePath)
	 - [fileName](#fileName) 

## ENUMS

### TokenKind

> Describes the kind of token.

##### VALUES

 * **Punctuation**: A token that contains some kind of punctuation.
 * **Keyword**: A language keyword.
 * **Identifier**: An identifier (that is not a keyword).
 * **Literal**: A numeric, string or character literal.
 * **Comment**: A comment.

## METHODS

### associatedNode()

> Returns the tokens associated node.

##### RETURNS

 * `ASTNode`

---

### name()

> Returns the token name.

##### RETURNS

 * `String`

---

### kind()

> Returns the token kind.

##### RETURNS

 * `Token.TokenKind`

---

### before(value)

 > Inserts value before this token.

##### PARAMETERS

 * **value** `String` Value to be inserted.

---

### after(value)

> Inserts value after this token.

##### PARAMETERS

 * **value** `String` Value to be inserted.

---

### afterln(value)

> Inserts value on the next line following this token. If the next line does not exist, the value is inserted after this token.

##### PARAMETERS

 * **value** `String` Value to be inserted. 
