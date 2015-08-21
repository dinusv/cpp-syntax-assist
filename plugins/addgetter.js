function addGetter(properties, node, save){

    var isInline = false;
    var isConst  = false;
    var isRef    = false;
    if ( typeof properties !== 'undefined' ){
        isInline = properties.indexOf('i') !== -1;
        isConst  = properties.indexOf('c') !== -1;
        isRef    = properties.indexOf('r') !== -1;
    }

    function extractFieldName(fieldDeclaration){
        if ( fieldDeclaration.indexOf('_') === 0 )
            return fieldDeclaration.slice(1);
        else if ( fieldDeclaration.indexOf('m_') === 0 )
            return fieldDeclaration.slice(2);
        else if ( fieldDeclaration.indexOf('_') === fieldDeclaration.length - 1)
            return fieldDeclaration.slice(0, fieldDeclaration.length - 1);
        return fieldDeclaration;
    }

    function findNode(node){
        if ( typeof node !== 'undefined' )
            return node.typeName() === 'field' ? node : null;
        else if ( codeBase.cursorNode() !== null )
            return codeBase.cursorNode().typeName() === 'field' ? codeBase.cursorNode() : null;
        return null;
    }

    function declareGetter(classNode, declaration){
        var publicAccess = classNode.lastChild('public', 'access');
        if ( publicAccess !== null )
            publicAccess.afterln('\n    ' + declaration + '\n');
        else
            classNode.append('\npublic:\n    ' + declaration + '\n');
    }

    // Node from parameter or codeBase
    // -------------------------------

    var fieldNode = findNode(node);
    if ( fieldNode === null )
        throw new Error("Selected node is not of \'field\' type");

    // Node identifier data
    // --------------------

    var nodeName          = fieldNode.identifier();
    var nodeExtractedName = extractFieldName(nodeName);

    var getterDeclaration =
            (isRef ? 'const ' + fieldNode.prop("type") + '& ' : fieldNode.prop("type") + ' ') +
            nodeExtractedName + '()' +
            (isConst ? ' const;' : ';');

    var fieldClass = fieldNode.astParent();
    var fieldClassName = fieldClass.identifier();

    // Add declaration
    // ---------------

    declareGetter(fieldClass, getterDeclaration);

    // Find namespace
    // --------------

    var breadCrumbs = '';
    var currentNode = fieldClass.astParent();
    while ( currentNode.typeName() !== 'file' ){
        breadCrumbs += currentNode.identifier() + '/';
        currentNode = currentNode.astParent();
    }

    if ( isInline ){
        fieldClass.afterln(
            '\ninline ' +
            (isRef ? 'const ' + fieldNode.prop("type") + '& ' : fieldNode.prop("type") + ' ') +
            fieldClass.identifier() + '::' + nodeExtractedName + '()' +
            (isConst ? ' const{' : '{') +
            '\n    return ' + nodeName + ';'
            + '\n}\n'
        );
    } else {

        // Add to source file
        // ------------------

        if ( currentNode.typeName() === 'file' ){
            var sourceFileNode = codeBase.findSource(currentNode.identifier());
            if ( sourceFileNode === null ){
                fieldClass.afterln(
                    '\ninline ' +
                    (isRef ? 'const ' + fieldNode.prop("type") + '& ' : fieldNode.prop("type") + ' ') +
                    fieldClass.identifier() + '::' + nodeExtractedName + '()' +
                    (isConst ? ' const{' : '{') +
                    '\n    return ' + nodeName + ';'
                    + '\n}\n'
                );
            } else {
                var usedNode = sourceFileNode;
                var cppBreadCrumbs = '';
                while ( breadCrumbs !== '' ){
                    var foundNode = sourceFileNode.findFirst(breadCrumbs);
                    if ( foundNode === null ){
                        var slashBackIndex = breadCrumbs.lastIndexOf('/', breadCrumbs.length - 2);

                        if ( slashBackIndex > 0 ){
                            cppBreadCrumbs =
                                cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
                            breadCrumbs    = breadCrumbs.slice(0, slashBackIndex + 1);
                        } else {
                            cppBreadCrumbs =
                                cppBreadCrumbs + breadCrumbs.slice(slashBackIndex + 1, breadCrumbs.length - 1) + '::';
                            breadCrumbs    = '';
                        }

                    } else {
                        usedNode = foundNode;
                        break;
                    }
                }

                usedNode.append(
                    '\n' +
                    (isRef ? 'const ' + fieldNode.prop("type") + '& ' : fieldNode.prop("type") + ' ') +
                    cppBreadCrumbs + fieldClass.identifier() + '::' + nodeExtractedName + '()' +
                    (isConst ? ' const{' : '{') +
                    '\n    return ' + nodeName + ';' +
                    '\n}\n'
                );
            }
        }
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addGetter',
    'usage' : 'addGetter(\'\')',
    'description' :
        'Adds a getter to the class field node.\n' +
        'Params:\n' +
            '\t * options <string> : enable flags [\'i\' - inline, \'c\' - constant, \'r\' - reference]'
}).prototype.addGetter = function(properties){
    this.nodes.forEach(function (v, i){
        addGetter(properties, v, false)
    });
    codeBase.save();
}

