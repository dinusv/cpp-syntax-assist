
NodeCollection.describe({
    'name' : 'addNamespace(name)',
    'usage' : 'addNamespace(\'name\')',
    'description' : 'Adds a namespace to the selected file or namespace.'
}).extend({
    'addNamespace' : function(name){
        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'file' || node.typeName() !== 'namespace' )
                throw new Error("Node is not of file|namespace type.")

            var lastParentNodeChild = node.lastChild('*', '')
            if ( lastParentNodeChild === null ){
                if ( node.typeName() === 'file' ){

                    var fileData      = node.readAll()
                    var macroEndIndex = fileData.lastIndexOf('#endif')

                    if ( macroEndIndex !== -1 ){
                        var macroEndPosition = node.createLocation(macroEndIndex)
                        node.insert('namespace ' + name + '{\n}\n', macroEndPosition)
                    } else {
                        node.append('\nnamespace ' + name + '{\n}')
                    }
                } else {
                    node.append('\nnamespace ' + name + '{\n}');
                }
            } else {
                lastParentNodeChild.after('\nnamespace ' + name + '{\n}')
            }
        });
        codeBase.save()
    }
})
