require('resolution.js')

NodeCollection.describe({
    name : 'setSinleton()',
    usage : 'setSingleton()',
    description : 'Generates singleton features to the current class.'
}).extend({
    setSingleton : function(properties){

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

        this.nodes.forEach(function (node, i){
            if ( node.typeName() === 'class' )
                throw new Error('Cannot find specified class.')

            declarePrivateMembers(node)
            declarePublicMembers(node)

            var sourceFileNode = codeBase.findSource(node.parentFind('file').identifier());
            if ( sourceFileNode === null ){
                node.afterln(
                    '\ninline ' + node.identifier() + '& ' + node.identifier() + '::' + 'instance(){\n' +
                    '    static ' + node.identifier() + ' inst;\n' +
                    '    return inst;\n' +
                    '}'
                );
            } else {
                var resolution = nodes(node).getResolution()
                var resolutionAndPosition = nodes(sourceFileNode).findClosestToResolution(resolution)

                resolutionAndPosition.node.append(
                    '\n' + node.identifier() + '& ' + resolutionAndPosition.resolutionString +
                    node.identifier() + '::' + 'instance(){\n' +
                    '    static ' + node.identifier() + ' inst;\n' +
                    '    return inst;\n' +
                    '}'
                );
            }

        });
        codeBase.save()
    }
})
