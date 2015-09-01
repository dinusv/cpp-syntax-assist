# ASTFile

**EXTENDS**: [ASTNode](#ASTNode)

Describes a c/c++ header or source file. Its located at the root of the Abstract Syntax Tree for each file. Use the codeBase to get the ASTFile for a parsed path:

```js
var file = codeBase.findFile("path/to/file");
```

Adding or reparsing ASTFiles is done through the codeBase property.

## INDEX

 - [METHODS](#methods)
     - [insert](#insert)
     - [erase](#erase)
     - [readAll](#readAll)
     - [read](#read)
     - [size](#size)
     - [createLocation](#createLocation)
     - [fileName](#fileName)
     - [fileNameWithoutExtension](#fileNameWithoutExtension)
     - [extension](#extension)

## METHODS

### insert(value, location)

> Inserts value at the specified location. Note that the changes do not happen immediately. Call `codeBase.save()` to save changes.

##### PARAMETERS

 * **value** `String` The value to insert.
 * **location** `SourceLocation` The location to insert the value to.

##### RETURNS

 * `Boolean` True if the insertion was possible, false otherwise.

---

### erase(from, to)

> Erase data between and including the specified positions. Similarly to the insertion, changes happen only after saving changes from the codeBase: `codeBase.save()`.

##### PARAMETERS

 * **from** `SourceLocation` The starting position.
 * **to** `SourceLocation` The end position.

##### RETURNS

 * `Boolean` True if the erase is possible, false otherwise.

---

### readAll()

> Read the file contents.

##### RETURNS

 * `String`

---

### read(from, to)

> Read the file contents between and including the two positions.

##### PARAMETERS

 * **from** `SourceLocation` The starting position.
 * **to** `SourceLocation` The end position.`

##### RETURNS

 * `String`

---

### size()

> Returns the file size.

##### RETURNS

 * `Number`

---

### createLocation(lineOrOffset, [column])

> Create a `SourceLocation` within the file by either specifying the offset, or the line and column. Columns and lines start from `1`. Offset, lines or columns are truncated to the maximum possible values within the file.

##### PARAMETERS

 * **lineOrOffset** `Number` The line or offset within the file.
 * **column** `Number` The column within the file. 

##### RETURNS

 * `SourceLocation`

---

### fileName()

> Get the file name. To get the file including the full path, use the `identifier()` method.

##### RETURNS

 * `String`

---

### fileNameWithoutExtension()

> Get the file name without its extension.

##### RETURNS

 * `String`

---

### extension()

> Get the file extension.

##### RETURNS

 * `String`

