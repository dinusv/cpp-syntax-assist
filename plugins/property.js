
function property(type, name, options){

    function capitaliseFirstLetter(str){
        return str.charAt(0).toUpperCase() + str.slice(1);
    }

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

    var propertyDeclaration = 'm_' + name;
    var isNotifiable        = options.indexOf('n') !== -1;
    var isReference         = options.indexOf('r') !== -1;
    //var isQProperty         = options.indexOf('q');

    var member     = '\t' + type + ' m_' + name + ';\n';
    var methodDecl =
            '\t' + (isReference ? 'const ' : '') + type +
            (isReference ? '& '   : '') + name  +
            '() const;\n' +

            '\tvoid set' + capitaliseFirstLetter(name) + '(' +
            (isReference ? 'const ' : '') +
            type +
            (isReference ? '& ' : ' ') +
            name + ');\n';

    var methodImpl =
            '\ninline ' + (isReference ? 'const ' : '') + type +
            (isReference ? '& ' : ' ') + classNode.identifier() + '::' + name +
            '() const{' + '\n\treturn m_' + name + ';\n}\n\n' +

            'inline void ' + classNode.identifier() + '::set' + capitaliseFirstLetter(name) +
            '(' + (isReference ? 'const ' : '') + type + (isReference ? '& ' : '') +
            name + '){\n\t';

    if ( isNotifiable ){
        methodImpl +=
                'if (m_' + name  + ' != ' + name + '){\n' +
                '\t\tm_' + name + ' = ' + name + ';\n' +
                '\t\temit ' + name + 'Changed();\n' +
                '\t}\n';
    } else {
        methodImpl +=
                '\tm_' + propertyDeclaration + ' = ' + propertyDeclaration + ';\n';
    }
    methodImpl += '}\n';

    classNode.afterln(methodImpl);

    var privateAccess = classNode.astChild('access', 'private');
    if ( privateAccess !== null ){
        var privateDeclarations = privateAccess.astChildren();
        if ( privateDeclarations.length > 0 )
            privateDeclarations[privateDeclarations.length - 1].after('\n' + member);
        else
            privateAccess.after('\n' + member);
    } else {
        classNode.append('private:\n' + member);
    }

    var publicAccess = classNode.astChild('access', 'public');
    if ( publicAccess !== null ){
        var publicDeclarations = publicAccess.astChildren();
        if ( publicDeclarations.length > 0 )
            publicDeclarations[publicDeclarations.length - 1].afterln(methodDecl + '\n');
        else
            publicAccess.after('\n' + methodDecl);
    } else {
        classNode.append('\npublic:\n' + methodDecl);
    }

    codeBase.save();
}
