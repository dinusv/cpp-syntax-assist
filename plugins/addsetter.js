require('resolution.js')

NodeCollection.describe({
    'name' : 'addSetter(flags)',
    'usage' : 'addSetter(\'i\')',
    'description' :
        'Adds a setter to the class field node.\n' +
        'Params:\n' +
            '\t * isInline <bool> : set to true to output inline setter (default: false)'
}).extend({
    addSetter: function(isInline){

        var addInline = (typeof isInline !== 'undefined') ? isInline : false;

        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'field' )
                throw new Error("Node is not of type field: " + node.identifier())

            // Node identifier data
            // --------------------

            var nodeName    = node.identifier();
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
            var setterDeclaration =  '    void ' + setterName + '(' + fieldNode.prop("type") + ' ' + nodeArgName + ');\n';

            var fieldClass     = node.astParent();
            var fieldClassName = fieldClass.identifier();

            // Add declaration
            // ---------------

            var publicAccess = fieldClass.lastChild('public', 'access');
            if ( publicAccess !== null )
                publicAccess.afterln(setterDeclaration);
            else
                fieldClass.append('\npublic:\n' + setterDeclaration);


            if ( addInline ){
                fieldClass.afterln(
                    '\n' + 'inline void ' + fieldClass.identifier() + '::' + setterName +
                            '(' + node.prop("type") + ' ' + argName + '){' +
                    '\n\t' + node.identifier() + ' = ' + argName + ';' +
                    '\n}\n'
                );
            } else {
                var sourceFileNode = codeBase.findSource(node.parentFind('file').identifier());
                if ( sourceFileNode === null ){
                    fieldClass.afterln(
                        '\n' + 'inline void ' + fieldClass.identifier() + '::' + setterName +
                                '(' + node.prop("type") + ' ' + argName + '){' +
                        '\n\t' + node.identifier() + ' = ' + argName + ';' +
                        '\n}\n'
                    );
                } else {
                    var resolution = nodes(node).getResolution()
                    var resolutionAndPosition = nodes(sourceFileNode).findClosestToResolution(resolution)

                    resolutionAndPosition.node.append(
                        '\n' + 'void ' + resolutionAndPosition.resolutionString + fieldClass.identifier() + '::' + setterName +
                                '(' + fieldNode.prop("type") + ' ' + nodeArgName + '){' +
                        '\n\t' + fieldNode.identifier() + ' = ' + nodeArgName + ';' +
                        '\n}\n'
                    );
                }
            }
        });
        codebase.save();
    }
})
