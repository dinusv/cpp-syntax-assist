
function destructor(){

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

    var destructorDeclaration = '\t~' + classNode.identifier() + '();\n';
    var destructorDefinition  = classNode.identifier() + '::~' + classNode.identifier() + '(){\n}\n';

    classNode.afterln('\n' + destructorDefinition);

    var constructorArray = classNode.astChildren('constructor');
    if ( constructorArray.length > 0 ){
        constructorArray[constructorArray.length - 1].afterln(destructorDeclaration + '\n');
    } else {
        var publicAccess = classNode.astChild('access', 'public');
        if ( publicAccess !== 0 ){
            publicAccess.afterln(destructorDeclaration + '\n');
        } else {
            classNode.append('\npublic:\n' + destructorDeclaration);
        }
    }
    codeBase.save();
}
