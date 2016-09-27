
NodeCollection.describe({
    'name' : 'addClass(name)',
    'usage' : 'addClass(\'name\')',
    'description' :
        'Adds a class to the selected file, namespace or class.'
}).extend({
    'addClass' : function(name){
        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'file' || node.typeName() !== 'namespace' || node.typeName() !== 'class' )
                throw new Error("Node is not of file|namespace|class type.")

            var lastParentNodeChild = node.lastChild('*', '')
            if ( lastParentNodeChild === null ){
                if ( node.typeName() === 'file' ){

                    var fileData      = node.readAll()
                    var macroEndIndex = fileData.lastIndexOf('#endif')

                    if ( macroEndIndex !== -1 ){
                        var macroEndPosition = node.createLocation(macroEndIndex)
                        node.insert('class ' + name + '{\n};\n', macroEndPosition)
                    } else {
                        node.append('\nclass ' + name + '{\n};')
                    }
                } else {
                    node.append('\nclass ' + name + '{\n};');
                }
            } else {
                lastParentNodeChild.afterln('class ' + name + '{\n};\n')
            }
        });
        codebase.save()
    }
})
