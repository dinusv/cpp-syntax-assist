
function preventCopy(){

    var cursorNode = codeBase.cursorNode();
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

    if ( classNode === null ){
        print("Cannot find specified class");
        return;
    }

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
