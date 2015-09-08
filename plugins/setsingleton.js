
function setSingleton(node, save){

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

    function declarePrivateMembers(classNode){
        var privateMemberDeclaration =
                '    ' + classNode.identifier() + '(' + ');\n' +
                '    ' + classNode.identifier() + '(const ' + classNode.identifier() + '&);\n' +
                '    ' + classNode.identifier() + '& operator =(const ' + classNode.identifier() + '&);\n';

        var privateAccess = classNode.firstChild('private', 'access');
        if ( privateAccess !== null ){
            privateAccess.after('\n' + privateMemberDeclaration + '\n');
        } else {
            classNode.append('\nprivate:\n    ' + privateMemberDeclaration);
        }
    }

    function declarePublicMembers(classNode){
        var publicMemberDeclaration = 'static ' + classNode.identifier() + '& instance();';

        var privateAccess = classNode.firstChild('public', 'access');
        if ( privateAccess !== null ){
            privateAccess.after('\n    ' + publicMemberDeclaration + '\n');
        } else {
            classNode.append('\npublic:\n    ' + publicMemberDeclaration + '\n');
        }
    }

    function getSourceNamespaceAndPosition(classNode){
        var breadCrumbs = '';
        var currentNode = classNode.astParent();
        while ( currentNode.typeName() !== 'file' || currentNode === null ){
            breadCrumbs = currentNode.identifier() + '/' + breadCrumbs;
            currentNode = currentNode.astParent();
        }
        if ( currentNode === null )
            return null;

        var sourceFileNode = codeBase.findSource(currentNode.identifier());
        if ( sourceFileNode === null )
            return null;

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
        return { node: usedNode, cppBreadCrumbs: cppBreadCrumbs };
    }

    var classNode = findNode();
    if ( classNode === null )
        throw new Error("Cannot find specified class.");

    declarePrivateMembers(classNode)
    declarePublicMembers(classNode)

    var namespaceAndPosition = getSourceNamespaceAndPosition(classNode);
    if ( namespaceAndPosition === null ){
        classNode.afterln(
            '\ninline ' + classNode.identifier() + '& ' + classNode.identifier() + '::' + 'instance(){\n' +
            '    static ' + classNode.identifier() + ' inst;\n' +
            '    return inst;\n' +
            '}'
        );
    } else {
        namespaceAndPosition.node.append(
            '\n' + classNode.identifier() + '& ' + namespaceAndPosition.cppBreadCrumbs +
            classNode.identifier() + '::' + 'instance(){\n' +
            '    static ' + classNode.identifier() + ' inst;\n' +
            '    return inst;\n' +
            '}'
        );
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'setSinleton()',
    'usage' : 'setSingleton()',
    'description' : 'Generates singleton features to the current class.'
}).prototype.setSingleton = function(properties){
    this.nodes.forEach(function (v, i){
        setSingleton(v, false)
    });
    codeBase.save()
}
