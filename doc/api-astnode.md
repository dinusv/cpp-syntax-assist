# ASTNode

Describes a node within the Abstract Syntax Tree. Nodes are acquired by traversing the tree from the `codeBase` object. The section will describe methods on retrieving node properties, traversal of nodes, and modifying node data.

A node is usually described by its typeName and identifier. The typeName can refer to the actual type of the node, which is either a class, namespace, field, etc. The identifier is the actual name the node is identified with in the AST. Custom properties can be retrieved for each node individually by using the [prop()](#prop) function.

From each node, one can visit its parent, children, and siblings. Custom search can be done by using a search pattern.

Values can be appended around certain points of the node, or custom insertion locations can be created around the node tokens, and around the node retrievable locations.

## INDEX

 - [METHODS](#methods)
    - [typeName](#typeName)
    - [identifier](#identifier)
    - [breadcrumbs](#breadcrumbs)
    - [description](#description)
    - [prop](#prop)
    - [text](#text)
    - [arguments](#arguments)
    - [associatedTokens](#associatedTokens)
    - [astParent](#astParent)
    - [firstChild](#firstChild)
    - [lastChild](#lastChild)
    - [children](#children)
    - [next](#next)
    - [prev](#prev)
    - [findNode](#findNode)
    - [find](#find)
    - [findFirst](#findFirst)
    - [parentFind])(#parentFind)
    - [append](#append)
    - [prepend](#prepend)
    - [before](#before)
    - [after](#after)
    - [afterln](#afterln)
    - [remove](#remove)
    - [rangeStartLocation](#rangeStartLocation)
    - [rangeEndLocation](#rangeEndLocation)
    - [cursorLocation](#cursorLocation)
    - [bodyStartLocation](#bodyStartLocation)
    - [bodyEndLocation](#bodyEndLocation)

## METHODS

### typeName()

> Returns the node typename ('class', 'namespace', 'field', etc).

##### RETURNS

 * `String`

---

### identifier()

> Returns the identifier of the node within the AST.

##### RETURNS

 * `String`

---

### breadcrumbs()

> Returns the nodes described location within the AST. The nodes breadcrumbs can be used as a search pattern to retrieve the node later from the codeBase.

```js
var breadcrumbs = node.breadcrumbs()

// Retrieve the node
var node = codeBase.find(breadcrumbs);
```

##### RETURNS

 * `String`

---

### description()

> Returns the node description. This is usually the identifier together with the properties of the node.

##### RETURNS

 * `String`

---

### prop(key)

> Returns the node property identifier by key.

##### PARAMETERS

 * **key** `String` Key of the property

##### RETURSN

 * `String`

---

### text()

> Returns the contents of the node between the node ranges. This is equivalent to reading the file between the node ranges:

```js
var nodeStart = node.rangeStartLocation()
var nodeEnd   = node.rangeEndLocation()

var text = codeBase.findFile(nodeStart.filePath()).read(nodeStart, nodeEnd);
```

##### RETURNS

 * `String`

---

### arguments()

> Returns the node arguments if the node has any (e.g. *Method* and *Constructor* nodes).

##### RETURNS

 * `Array`

---

### associatedTokens()

> Returns an array of the nodes associated tokens.

##### RETURNS

 * `Array`

---

### astParent()

> Returns the parent of this node.

##### RETURNS

 * `ASTNode`

### firstChild([identifier, typeString])

> Returns the first child of this node that matches the identifier and typestring if they have been specified. Otherwise returns the first child of this node.

##### PARAMETERS

 * **identifier** `String` The identifier to search for.
 * **typeString** `String` The type of the node to match.

##### RETURNS

 * `ASTNode` The found node. `null` if not found.

---

### lastChild([identifier, typeString])

> Returns the last child of this node that matches the identifier and typestring if they have been specified. Otherwise returns the last child of this node.

##### PARAMETERS

 * **identifier** `String` The identifier to search for.
 * **typeString** `String` The type of the node to match.

##### RETURNS

 * `ASTNode` The found node. `null` if not found.

---

### children([type])

> Returns the children of this node matching the *type*. If no type is given, all the children are returned.

##### PARAMETERS

 * **type** `String` The type of nodes to match


##### RETURNS

 * `Array` Array of *ASTNodes*.

---

### next()

> Returns the next sibling of this node.

##### RETURNS

 * `ASTNode` The sibling node. `null` if there is none.

---

### prev()

> Returns the previous sibling of this node.

##### RETURNS

 * `ASTNode` The sibling node. `null` if there is none.

---

### findNode(node)

> Search for the specified node recursively within this nodes children. Usefull in checking wether a node is a child of this one.

##### PARAMETERS

 * **node** `ASTNode` The node to search for.

##### RETURNS

 * `ASTNode` The found node. `null` if none was found.

---

### find(searchData, [type])

---

### findFirst(searchData, [type])

---

### parentFind(type)

---

### append(value)

---

### prepend(value)

---

### before(value)

---

### after(value)

---

### afterln(value)

---

### remove()

---

### rangeStartLocation()

---

### rangeEndLocation()

---

### cursorLocation()

---

### bodyStartLocation()

---

### bodyEndLocation()



		
