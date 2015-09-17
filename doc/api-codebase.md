# CodeBase

Class for the main `codeBase` object. Handles file parsing, and manages the directory structure. Contains a reference to a selected node sent through CSAs command line arguments.

## INDEX

 - [METHODS](#methods)
     - [createLocation(file, lineOrOffset, column](#createlocationfilelineoroffsetcolumn)
     - [save()](#save)
     - [select(searchData, type)](#selectsearchdatatype)
     - [selectNode(node)](#selectnodenode)
     - [selectedNode()](#selectednode)
     - [setProjectDir(path)](#setprojectdirpath)
     - [find(searchData, type)](#findsearchdatatype)
     - [findFile(filePath)](#findfilepath)
     - [findSource(headerFile)](#findsourceheaderfile)
     - [findHeader(sourceFile)](#findheadersourcefile)
     - [files()](#files)
     - [parsePath(path)](#parsepathpath)
     - [parseFile(file)](#parsefilefile)
     - [reparseFile(file)](#reparsefilefile)
     - [createFile(filePath)](#createfilefilepath)
     - [makePath(path)](#makepathpath)

## METHODS

### createLocation(file, lineOrOffset, [column])

> Create a `SourceLocation` within the file by either specifying the offset, or the line and column. Columns and lines start from `1`. Offset, lines or columns are truncated to the maximum possible values within the file.

##### PARAMETERS

 * **file** `String` File path.
 * **lineOrOffset** `Number` The line or offset within the file.
 * **column** `Number` The column within the file. 

##### RETURNS

 * `SourceLocation`

---

### save()

> Save and reparse all files that have changes. (Warning: this makes all references to current nodes invalid).

---

### select(searchData, [type])

> Select the first node matching the search pattern.

##### PARAMETERS

 * **searchData** `String` Search pattern.
 * **type** The node will be matched against the type property if present.

##### RETURNS

 * `Boolean` True if match was found and node was selected, false otherwise.

---

### selectNode(node)

> Select the specified node.

##### PARAMETERS

 * **node** `ASTNode` Node to select.

##### RETURNS

 * `Boolean` True if node is selected (or `node !== null`), false otherwise.

---

### selectedNode()

> Get the selected node.

##### RETURNS

 * `ASTNode` The selected node, or `null` if no node is selected.

---

### setProjectDir(path)

> Set the project directory. The project directory is used to parse or find files.

##### PARAMETERS

 * **path** `String` The path to the project directory. Can be relative or absolute to the current project directory.

---

### find(searchData, [type])

> Find the set of nodes recursively matching the specified search pattern.

##### PARAMETERS

 * **searchData** `String` Search pattern
 * **type** `String` Only nodes of the speicifed type will be returned if the type is present.

##### RETURNS

 * `Array` List of found ASTNodes.

---

### findFile(filePath)

> Get the loaded specified file node according to its path.

##### PARAMETERS

 * **filePath** `String` The path and name of the file.

##### RETURNS

 * `ASTFile` Found file node, `null` otherwise.

---

### findSource(headerFile)

> Find and parse the source file associated with the given header file path.

##### PARAMETERS

 * **headerFile** `String` Path to the header file.

##### RETURNS

 * `ASTFile` Found parsed file node, `null` otherwise. 

---
### findHeader(sourceFile)

> Find and parse the header file associated with the given source file path.

##### PARAMETERS

 * **sourceFile** `String` Path to the source file.

##### RETURNS

 * `ASTFile` Found parsed file node, `null` otherwise. 

---

### files()

> Returns all the files parsed by the codeBase.

##### RETURNS

 * `Array` Array of `ASTFile` nodes.

---

### parsePath(path)

> Scans the given path recursively and parses all header and source files and adds them to the codeBase. If the path is a file, the file is parsed and added to the codebase.

##### PARAMETERS

 * **path** `String` Path to parse.

---

### parseFile(file)

> Parses the given file path and returns the associated node.

##### PARAMETERS

 * **file** `String` Path to the file, which can be either relative to the current project directory, or absolute.

##### RETURNS

 * `ASTFile` The file node if the file has been parsed correctly, `null` otherwise.

---

### reparseFile(file)

> Reparses the file node, and updates the codeBase.

##### PARAMETERS

 * **file** `ASTFile` The file node to reparse.

##### RETURNS

 * `ASTFile` the reparsed file, `null` if the file could not be reparsed. 

---

### createFile(filePath)

> Creates an empty file at the given path, and returns the node reference to the given file.

##### PARAMETERS

 * **filePath** `String` Path to the file, which can be either relative to the current project directory, or absolute. The directory in which is created must exist, and have the appropriate permissions.

##### RETURNS

 * `ASTFile` the file node, `null` if the file could not be created.

---

### makePath(path)

> Create the given path recursively.

##### PARAMETERS

 * **path** `String` The path to create. This can be relative to the current project directory or absolute.

###### RETURNS

 * `Boolean` True if created, false otherwise.
 
