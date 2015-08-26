
function addClass(name, node, save){

    function findNode(){
        if (typeof node === 'undefined'){
            var cursorNode = codeBase.selectedNode();
            if ( cursorNode.typeName() === 'file' ||
                 cursorNode.typeName() === 'namespace' ||
                 cursorNode.typeName() === 'class'
            )
                return cursorNode;
            else
                return null;
        } else {
            return (
                node.typeName() === 'file' ||
                node.typeName() === 'namespace' ||
                node.typeName() === 'class' ? node : null);
        }
    }

    var usedNode = findNode()
    if ( usedNode === null )
        throw new Error("Cannot find parent to add class to.");

    var lastParentNodeChild = usedNode.lastChild('*', '')
    if ( lastParentNodeChild === null ){
        if ( usedNode.typeName() === 'file' ){

            var fileData      = usedNode.readAll()
            var macroEndIndex = fileData.lastIndexOf('#endif')

            if ( macroEndIndex !== -1 ){
                var macroEndPosition = usedNode.createLocation(macroEndIndex)
                usedNode.insert('class ' + name + '{\n};\n', macroEndPosition)
            } else {
                usedNode.append('\nclass ' + name + '{\n};')
            }
        } else {
            usedNode.append('\nclass ' + name + '{\n};');
        }
    } else {
        lastParentNodeChild.after('\nclass ' + name + '{\n};')
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addClass',
    'usage' : 'addClass(\'name\')',
    'description' :
        'Adds a class to the selected file, namespace or class.'
}).prototype.addClass = function(name){
    this.nodes.forEach(function (name, v, i){
        addClass(name, v, false)
    });
    codeBase.save()
}
