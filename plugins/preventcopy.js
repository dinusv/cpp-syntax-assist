
NodeCollection.describe({
    'name' : 'preventCopy()',
    'usage' : 'preventCopy()',
    'description' : "Adds private copy constructor to class."
}).extend({
    'preventCopy': function(){
        this.nodes.forEach(function (node, i){
            if ( node.typeName() !== 'class' )
                node = node.parentFind('class')

            if ( node === null )
                throw new Error("Cannot find specified class");

            var preventCopyText =
                '    ' + node.identifier() + '(const ' + node.identifier() + '& other);\n' +
                '    ' + node.identifier() + '& operator = (const ' + node.identifier() + '& other);\n\n';

            var privateAccess = node.firstChild('access', 'private');
            if ( privateAccess !== null ){
                privateAccess.afterln(preventCopyText);
            } else {
                node.append('\nprivate:\n' + preventCopyText);
            }
        });
        codeBase.save()
    }
})
