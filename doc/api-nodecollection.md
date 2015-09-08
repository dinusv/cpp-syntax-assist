# NodeCollection

The NodeCollection class defines a collection of nodes to apply plugins to. The collection is initialized through the nodes function, where a selector is used to create the actual node array.

```js
nodes('Date', 'class').addField('int', 'm_month');
```

The collection also manages the actual plugins, by providing a function for them to register and describe themselves for CSAs auto-completion feature.

The prototype property is then used to add the actual function. The local nodes stored within the collection are then sent to the function as arguments. If the function can process only a single node at a time, the nodes need to be iterated and sent separately.

```js
function addField(type, name){
    // Add Field Implementation...
}

NodeCollection.registerPlugin({
    'name' : 'addField',
    'usage' : 'addField(\'name\')',
    'description' :
        'Adds a field to the current class.' +
        'Params:\n' +
			'\ttype <string> : type of the field.' +
            '\tname <string> : name of the field.'
}).prototype.addField = function(type, name){
    this.nodes.forEach(function (v, i){
        addField(type, name, v)
    });
    codeBase.save()
}
```

Examples and more information on how to create and add plugins is available in the adding plugins section.

## INDEX

 - [PROPERTIES](#properties)
	 - [nodes](#nodes)
 - [METHODS](#methods)
	 - [children()](#children)
	 - [find()](#find-selector-type)
	 - [toString()](#toString)
	 - [registerPlugin(properties) `static`](#registerPlugin-properties-static)


## PROPERTIES

### nodes

`Array`

> Selected nodes found within this collection. 

## METHODS

### children()

> Returns a new NodeCollection containing all the children of the current collections nodes.

##### RETURNS

 * `NodeCollection`

---

### find(selector, [type])

> Returns a new NodeCollection containing all the found nodes from the current collections nodes.

##### PARAMETERS

 * selector `String` The selector to use.
 * type `String` The type of the nodes to select.

##### RETURNS

 * `NodeCollection`

---

### toString()

> Returns the representation of this NodeCollection.

##### RETURNS

 * `String`

---

### registerPlugin(properties) `static`

> Registers the plugin to CSA's plugin repository, which can be later accessed for auto-completion or documentation.

##### PARAMETERS

 * properties `Object` Contains the following fields:
	 * name: the name of the function
	 * usage *[optional]*: usage example for auto-completion
	 * description *[optional]*: description of the function and its parameters

##### RETURNS

 * Reference to NodeCollection
