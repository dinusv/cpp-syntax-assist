# SourceLocation

The class describes a location within a source code of a file. An instance can be created either from an (ASTFile)[api-astfile.md] node, or from the main (codeBase)[api-codebase.md] object.

```js

// Location from file offset
var location = codeBase.createLocation('date.h', 100);

// Location from line, column
var location = codeBase.createLocation('date.h', 10, 10);
```

```js
var loadedFile = codeBase.findFile('Date.hpp');

// Location from file offset
var location   = loadedFile.createLocation(100);

// Location from line, column
var location   = loadedFile.createLocation(10, 10);
```

Source locations can become invalid once the codeBase is saved. The creationLocation functions return null if the file does not exist, otherwise if the offset, line or colum values are incompatible, they will be truncated to create a valid location within the file.

## INDEX

 - [METHODS](#methods)
	 - [line()](#line)
	 - [column()](#column)
	 - [offset()](#offset)
	 - [filePath()](#filepath)
	 - [fileName()](#filename)

## METHODS

### line()

> Returns the line in the file associated with this location.

##### RETURNS

 * `Number`

---

### column()

> Returns the column in the file associated with this location.

##### RETURNS

 * `Number`

---

### offset()

> Returns the offset in the file associated with this location.

##### RETURNS

 * `Number`

---

### fileName()

> Returns the file name associated with this location.

##### RETURNS

 * `String`

---

### filePath()

> Returns the file name, including the path associated with this location.

##### RETURNS

 * `String`
