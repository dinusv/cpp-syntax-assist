# CSA JS Api

This section describes the js API used to create CSA plugins.

## INDEX

 - [PROPERTIES](#properties)
	 - [codeBase](#codebase)
	 - [console](#console)

 - [METHODS](#methods)
	 - [nodes(selector, type)](#nodesselectortype)
	 - [createFile(file)](#createfilefile)
	 - [parse(file)](#parsefilefile)
	 - [reparse(selector)](#reparseselector)
	 - [makePath(path)](#makepathpath)

 - CLASSES
	 - [Token](api-token.md)
	 - [SourceLocation](api-sourcelocation.md)
	 - [NodeCollection](api-nodecollection.md)
	 - [ASTNode](api-astnode.md)
	 - [ASTFile](api-astfile.md)
	 - [CodeBase](api-codebase.md)


## PROPERTIES

### codeBase

> The main codeBase object. Resembles the root of the AST hierarchy and manages user cursor positions.

##### TYPE

 * [CodeBase](api-codebase.md)

---

### console

> The console object is currently a minimal implementation of the standard javascript console object, containing only a few utility methods:

 * **log(message)**: outputs the message to the console
 * **warn(message)**: outputs the warning to the console
 * **error(message)**: outputs the error to the console

---

## METHODS

### nodes([selector, type])

> Create a NodeCollection with nodes matching the given selection or search pattern.

##### PARAMETERS

 * **selector** `String` The selector or search pattern to use.
 * **type** `String` The type of nodes to select.

##### RETURNS

 * `NodeCollection`

---

### createFile(file)

> Creates an empty file at the given path, and returns the node reference to the given file.

##### PARAMETERS

 * **file** `String` Path to the file, which can be either relative to the current project directory, or absolute. The directory in which is created must exist, and have the appropriate permissions.

##### RETURNS

 * `NodeCollection` the node collection containing the file node

---

### parseFile(file)

> Parses the given file path and returns a NodeCollection with the associated node.

##### PARAMETERS

 * **file** `String` Path to the file, which can be either relative to the current project directory, or absolute.

##### RETURNS

 * `NodeCollection` Contains the file node if the file has been found or parsed correctly, otherwise the Collection will be empty.

---

### reparse(selector)

> Finds the file matching the given selector, and reparses the file if found, updating the codeBase.

##### PARAMETERS

 * **selector** `String` Selector matching the file to be reparsed.

##### RETURNS

 * `NodeCollection` Contains the reparsed file, or is empty if the file is not found or could not be parsed.

---

### makePath(path)

> Creates the given set of directories recursively.

##### PARAMETERS

 * **path** `String` The path to create. This can be relative to the current project directory or absolute.

###### RETURNS

 * `Boolean` True if created, false otherwise.
