

NodeCollection.describe({
    'name' : 'addIncludeGuard()',
    'usage' : 'addIncludeGuard()',
    'description' : 'Adds an include guard to the current file.'
}).extend({
    'addIncludeGuard' : function(){
        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'file' )
                node = node.parentFind('file')

            node.prepend(
                '#ifndef ' + node.fileNameWithoutExtension().toUpperCase() + '_' + node.extension().toUpperCase() +
                '\n' +
                '#define ' + node.fileNameWithoutExtension().toUpperCase() + '_' + node.extension().toUpperCase() +
                '\n'
            );
            node.append('\n#endif')
        });
        codeBase.save()
    }
})
