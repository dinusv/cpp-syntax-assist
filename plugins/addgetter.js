require('resolution.js')

NodeCollection.describe({
    'name' : 'addGetter(options)',
    'usage' : 'addGetter(\'\')',
    'description' :
        'Adds a getter to the class field node.\n' +
        'Params:\n' +
            '\t * options <string> : enable flags [\'i\' - inline, \'c\' - constant, \'r\' - reference]'
}).extend({
    addGetter : function(properties){
        var isInline = false;
        var isConst  = false;
        var isRef    = false;
        if ( typeof properties !== 'undefined' ){
            isInline = properties.indexOf('i') !== -1;
            isConst  = properties.indexOf('c') !== -1;
            isRef    = properties.indexOf('r') !== -1;
        }

        this.nodes.forEach(function(node, i){
            if ( node.typeName() !== 'field' )
                throw new Error("Node is not of type field: " + node.identifier())

            // Node identifier data
            // --------------------

            var nodeName          = node.identifier();
            var nodeExtractedName = nodeName;

            if ( nodeName.indexOf('_') === 0 )
                nodeExtractedName = nodeExtractedName.slice(1);
            else if ( nodeName.indexOf('m_') === 0 )
                nodeExtractedName = nodeExtractedName.slice(2);
            else if ( nodeName.indexOf('_') === nodeName.length - 1)
                nodeExtractedName = nodeExtractedName.slice(0, nodeName.length - 1);

            var getterDeclaration =
                    (isRef ? 'const ' + node.prop("type") + '& ' : node.prop("type") + ' ') +
                    nodeExtractedName + '()' +
                    (isConst ? ' const;' : ';');

            var fieldClass = node.astParent();
            var fieldClassName = fieldClass.identifier();

            // Add Declaration
            // ---------------

            var publicAccess = fieldClass.lastChild('public', 'access');
            if ( publicAccess !== null )
                publicAccess.afterln('    ' + getterDeclaration + '\n');
            else
                fieldClass.append('\npublic:\n    ' + getterDeclaration + '\n');


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
                var sourceFileNode = codebase.findSource(node.parentFind('file').identifier());
                if ( sourceFileNode === null ){
                    fieldClass.afterln(
                        '\ninline ' +
                        (isRef ? 'const ' + node.prop("type") + '& ' : node.prop("type") + ' ') +
                        fieldClass.identifier() + '::' + nodeExtractedName + '()' +
                        (isConst ? ' const{' : '{') +
                        '\n    return ' + nodeName + ';'
                        + '\n}\n'
                    );
                } else {
                    var resolution = nodes(node).getResolution()
                    var resolutionAndPosition = nodes(sourceFileNode).findClosestToResolution(resolution)

                    resolutionAndPosition.node.append(
                        '\n' +
                        (isRef ? 'const ' + node.prop("type") + '& ' : node.prop("type") + ' ') +
                        resolutionAndPosition.resolutionString + fieldClass.identifier() + '::' + nodeExtractedName + '()' +
                        (isConst ? ' const{' : '{') +
                        '\n    return ' + nodeName + ';' +
                        '\n}\n'
                    )
                }
            }
        });
        codebase.save();
    }
});
