
function addDestructor(node, save){

    var classNode = null;

    if ( typeof node === 'undefined' ){

        var cursorNode = codeBase.cursorNode();

        if ( cursorNode.typeName() === 'class' )
            classNode = cursorNode;
        else {
            classNode = cursorNode.parentFind('class');
        }

        if ( classNode === null ){
            console.error("Cannot find 'class' node for insertion.")
        }
    } else {
        classNode = node.typeName() === 'class' ? node : null;
    }

    if ( classNode === null )
        throw "Cannot find specified class";

    var destructorDeclaration = '\t~' + classNode.identifier() + '();\n';
    var destructorDefinition  = classNode.identifier() + '::~' + classNode.identifier() + '(){\n}\n';

    classNode.afterln('\n' + destructorDefinition);

    var constructorArray = classNode.children('constructor');
    if ( constructorArray.length > 0 ){
        constructorArray[constructorArray.length - 1].afterln(destructorDeclaration + '\n');
    } else {
        var publicAccess = classNode.firstChild('access', 'public');
        if ( publicAccess !== null ){
            publicAccess.afterln(destructorDeclaration + '\n');
        } else {
            classNode.append('\npublic:\n' + destructorDeclaration);
        }
    }

    var saveCodebase = typeof save !== 'undefined' ? save : true;
    if (save)
        codeBase.save()
}

NodeCollection.registerPlugin({
    'name' : 'addDestructor',
    'usage' : 'addDestructor()',
    'description' : 'Adds a destructor to the current class or parent class.'
}).prototype.addDestructor = function(){
    this.nodes.forEach(function (v, i){
        addDestructor(v, false)
    });
    codeBase.save()
}
