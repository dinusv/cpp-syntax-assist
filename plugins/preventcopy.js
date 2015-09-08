
function preventCopy(){

    var cursorNode = codeBase.selectedNode();
    var classNode  = null;

    if ( cursorNode.typeName() === 'class' )
        classNode = cursorNode;
    else {
        classNode = cursorNode.parentFind('class');
    }

    if ( classNode === null ){
        var loadedFiles = codeBase.files();
        for (var index = 0; index < loadedFiles.length; ++index) {
            classNode = loadedFiles[index].findFirst('class');
            if ( classNode !== null )
                break;
        }
    }

    if ( classNode === null )
        throw "Cannot find specified class";

    var preventCopyText =
        '\t' + classNode.identifier() + '(const ' + classNode.identifier() + '& other);\n' +
        '\t' + classNode.identifier() + '& operator = (const ' + classNode.identifier() + '& other);\n\n';

    var privateAccess = classNode.astChild('access', 'private');
    if ( privateAccess !== null ){
        privateAccess.afterln(preventCopyText);
    } else {
        classNode.append('\nprivate:\n' + preventCopyText);
    }

    codeBase.save();
}

if ( typeof plugins !== 'undefined' ){
    plugins.registerPlugin({
        'name' : 'preventCopy()',
        'usage' : 'preventCopy()',
        'description' : 'adds the private copy constructor and assignment operator to the selected or parent class.'
    });
}
