require('resolution.js')

NodeCollection.describe({
    name : 'addDestructor()',
    usage : 'addDestructor()',
    description :
        'Adds a destructor to the current class or parent class.\n' +
        'Params:\n' +
            '\t\'i\' <bool> : \'i\' - to enable inline destructor.'
}).describe({
    name : 'declareDestructor()',
    usage : 'declareDestructor()',
    description :
        'Declares a destructor.'
}).extend({
    declareDestructor : function(){
        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'class' )
                throw new Error("Cannot find specified class.");

            var destructorDeclaration = '    ~' + node.identifier() + '();\n';
            var constructorArray = node.children('constructor');
            if ( constructorArray.length > 0 ){
                constructorArray[constructorArray.length - 1].afterln(destructorDeclaration + '\n');
            } else {
                var publicAccess = node.firstChild('public', 'access');
                if ( publicAccess !== null ){
                    publicAccess.after('\n' + destructorDeclaration + '\n');
                } else {
                    node.append('\npublic:\n' + destructorDeclaration);
                }
            }
        })
    },

    addDestructor : function(properties){
        this.declareDestructor()

        this.nodes.forEach(function (node, i){
            var isInline = false;
            if ( typeof properties !== 'undefined' )
                isInline = properties === 'i';

            if ( node.typeName() !== 'class' )
                throw new Error("Cannot find specified class.");

            if ( isInline ){
                node.afterln('\ninline ' + node.identifier() + '::~' + node.identifier() + '(){\n}\n');
            } else {
                var sourceFileNode = codeBase.findSource(node.parentFind('file').identifier());
                if ( sourceFileNode === null ){
                    node.afterln('\ninline ' + node.identifier() + '::~' + node.identifier() + '(){\n}\n');
                } else {
                    var resolution = nodes(node).getResolution()
                    var resolutionAndPosition = nodes(sourceFileNode).findClosestToResolution(resolution)

                    resolutionAndPosition.node.append(
                        '\n' + resolutionAndPosition.resolutionString + node.identifier() + '::~' +
                        node.identifier() + '(){\n}\n'
                    );
                }
            }
        });
        codebase.save()
    }
})

