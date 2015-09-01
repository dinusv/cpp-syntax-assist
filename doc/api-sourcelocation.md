# SourceLocation

The class describes a location within a source code of a file. An instance can be created either from an ASTFile node, or from the main codeBase.

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

Source locations can become invalid once the codeBase is saved. The creationLocation functions return null if the file does not exist, and the offset, line or colum values will be truncated to create a valid location within the file. 

## INDEX

 - [METHODS](#methods)
	 - [line](#line)
	 - [column](#column)
	 - [offset](#offset)
	 - [filePath](#filePath)
	 - [fileName](#fileName) 

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
