
function addNamespace(name, node, save){

    function findNode(){
        if (typeof node === 'undefined'){
            var cursorNode = codeBase.selectedNode();
            if ( cursorNode.typeName() === 'file' || cursorNode.typeName() === 'namespace' )
                return cursorNode;
            else
                return null;
        } else {
            return (node.typeName() === 'file' || node.typeName() === 'namespace' ? node : null);
        }
    }

    var usedNode = findNode()
    if ( usedNode === null )
        throw new Error("Cannot find parent to add namespace to.");

    var lastParentNodeChild = usedNode.lastChild('*', '')
    if ( lastParentNodeChild === null ){
        if ( usedNode.typeName() === 'file' ){

            var fileData      = usedNode.readAll()
            var macroEndIndex = fileData.lastIndexOf('#endif')

            if ( macroEndIndex !== -1 ){
                var macroEndPosition = usedNode.createLocation(macroEndIndex)
                usedNode.insert('namespace ' + name + '{\n}\n', macroEndPosition)
            } else {
                usedNode.append('\nnamespace ' + name + '{\n}')
            }
        } else {
            usedNode.append('\nnamespace ' + name + '{\n}');
        }
    } else {
        lastParentNodeChild.after('\nnamespace ' + name + '{\n}')
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addNamespace',
    'usage' : 'addNamespace()',
    'description' :
        'Adds a namespace to the selected file or namespace.'
}).prototype.addNamespace = function(name){
    this.nodes.forEach(function (name, v, i){
        addNamespace(name, v, false)
    });
    codeBase.save()
}
