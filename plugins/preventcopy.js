
function preventCopy(node, save){

    function findNode(){
        if (typeof node === 'undefined'){
            var cursorNode = codeBase.selectedNode();
            if ( cursorNode.typeName() === 'class' )
                return cursorNode;
            else
                return cursorNode.parentFind('class');
        } else {
            return (node.typeName() === 'class' ? node : null);
        }
    }

    var classNode = findNode()
    if ( classNode === null )
        throw new Error("Cannot find specified class");

    var preventCopyText =
        '    ' + classNode.identifier() + '(const ' + classNode.identifier() + '& other);\n' +
        '    ' + classNode.identifier() + '& operator = (const ' + classNode.identifier() + '& other);\n\n';

    var privateAccess = classNode.firstChild('access', 'private');
    if ( privateAccess !== null ){
        privateAccess.afterln(preventCopyText);
    } else {
        classNode.append('\nprivate:\n' + preventCopyText);
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'preventCopy()',
    'usage' : 'preventCopy()',
    'description' : "Adds private copy constructor to class."
}).prototype.preventCopy = function(){
    this.nodes.forEach(function (v, i){
        preventCopy(v, false)
    });
    codeBase.save()
}
