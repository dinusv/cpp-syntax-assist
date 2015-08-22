function addSetter(isInline, node, save){

    // Add inline setter function
    // --------------------------

    function addInlineSetter(classNode, fieldNode, setterName, argName){
        classNode.afterln(
            '\n' + 'inline void ' + classNode.identifier() + '::' + setterName +
                    '(' + fieldNode.prop("type") + ' ' + argName + '){' +
            '\n\t' + fieldNode.identifier() + ' = ' + argName + ';' +
            '\n}\n'
        );
    }

    // Node from parameter or codeBase
    // -------------------------------

    var fieldNode = null;

    if ( typeof node !== 'undefined' )
        fieldNode = node.typeName() === 'field' ? node : null;
    else if ( codeBase.cursorNode() !== null )
        fieldNode = codeBase.cursorNode().typeName() === 'field' ? codeBase.cursorNode() : null;

    if ( fieldNode === null )
        throw new Error("Selected node is not of \'field\' type");

    // Node identifier data
    // --------------------

    var nodeName    = fieldNode.identifier();
    var nodeArgName = nodeName;

    if ( nodeName.indexOf('_') === 0 )
        nodeArgName = nodeArgName.slice(1);
    else if ( nodeName.indexOf('m_') === 0 )
        nodeArgName = nodeArgName.slice(2);
    else if ( nodeName.indexOf('_') === nodeName.length - 1)
        nodeArgName = nodeArgName.slice(0, nodeName.length - 1);

    var setterName = 'set' + nodeArgName.charAt(0).toUpperCase() + nodeArgName.slice(1);

    if ( nodeArgName === nodeName )
        nodeArgName = 'arg';
    var setterDeclaration =  'void ' + setterName + '(' + fieldNode.prop("type") + ' ' + nodeArgName + ');\n';

    var fieldClass = fieldNode.astParent();
    var fieldClassName = fieldClass.identifier();

    // Add declaration
    // ---------------

    var publicAccess = fieldClass.lastChild('public', 'access');
    if ( publicAccess !== null )
        publicAccess.afterln(setterDeclaration);
    else
        fieldClass.append('\npublic:\n' + setterDeclaration);

    // Find namespace
    // --------------

    var breadCrumbs = '';
    var currentNode = fieldClass.astParent();
    while ( currentNode.typeName() !== 'file' ){
        breadCrumbs = currentNode.identifier() + '/' + breadCrumbs;
        currentNode = currentNode.astParent();
    }

    var addInline = (typeof isInline !== 'undefined') ? isInline : false;
    if ( addInline ){
        addInlineSetter(fieldClass, fieldNode, setterName, nodeArgName)
    } else {

        // Add to source file
        // ------------------

        if ( currentNode.typeName() === 'file' ){
            var sourceFileNode = codeBase.findSource(currentNode.identifier());
            if ( sourceFileNode === null )
                addInlineSetter(fieldClass, fieldNode, setterName, nodeArgName);
            else {
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
                    '\n' + 'void ' + cppBreadCrumbs + fieldClass.identifier() + '::' + setterName +
                            '(' + fieldNode.prop("type") + ' ' + nodeArgName + '){' +
                    '\n\t' + fieldNode.identifier() + ' = ' + nodeArgName + ';' +
                    '\n}\n'
                );
            }
        }
    }

    var saveCodebase = typeof save !== 'undefined' ? save : true;
    if (save)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addSetter',
    'usage' : 'addSetter()',
    'description' :
        'Adds a setter to the class field node.\n' +
        'Params:\n' +
            '\t * isInline <bool> : set to true to output inline setter (default: false)'
}).prototype.addSetter = function(isInline){
    this.nodes.forEach(function (v, i){
        addSetter(isInline, v, false)
    });
    codeBase.save();
}

