
function addIncludeGuard(node, save){

    function findNode(){
        if (typeof node === 'undefined'){
            var cursorNode = codeBase.selectedNode();
            if ( cursorNode.typeName() === 'file' )
                return cursorNode;
            else
                return cursorNode.parentFind('file');
        } else {
            return (node.typeName() === 'file' ? node : null);
        }
    }

    var fileNode = findNode();
    if ( fileNode === null )
        throw new Error("Cannot find file node.");

    fileNode.prepend(
        '#ifndef ' + fileNode.fileNameWithoutExtension().toUpperCase() + '_' + fileNode.extension().toUpperCase() +
        '\n' +
        '#define ' + fileNode.fileNameWithoutExtension().toUpperCase() + '_' + fileNode.extension().toUpperCase() +
        '\n'
    );
    fileNode.append('\n#endif')

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}


NodeCollection.registerPlugin({
    'name' : 'addIncludeGuard()',
    'usage' : 'addIncludeGuard()',
    'description' : 'Adds an include guard to the current file.'
}).prototype.addIncludeGuard = function(properties){
    this.nodes.forEach(function (v, i){
        addIncludeGuard(v, false)
    });
    codeBase.save()
}
