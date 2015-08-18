
function addDestructor(node){

    var classNode = null;

    if ( typeof node === 'undefined' ){

        var cursorNode = codeBase.cursorNode();

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

    codeBase.save();
}

function NodeCollection(nodes){this.nodes = nodes ? nodes : [];}

NodeCollection.prototype.children = function(){
    var newCollection = new NodeCollection();
    for ( var i = 0; i < this.nodes.length; ++i )
        newCollection.nodes.push(nodes[i].children());
    return newCollection;
}

NodeCollection.prototype.toString = function(){
    var ret = 'NodeCollection[';
    for ( var i = 0; i < this.nodes.length; ++i ){
        if ( i !== 0 )
            ret += ', ';
        ret += '\'' + this.nodes[i].content() + '\'';
    }
    return ret + ']';
}

NodeCollection.registerPlugin = function(properties){
    if( typeof plugins !== 'undefined' )
        plugins.registerPlugin(properties);
    return NodeCollection;
}

function nodes(selector, type){
    return new NodeCollection(codeBase.find(selector, type ? type : ''));
}

NodeCollection.registerPlugin({
    'name' : 'addDestructor',
    'usage' : 'addDestructor()',
    'description' : 'Adds a destructor to the current class or parent class.'
}).prototype.addDestructor = function(){
    this.nodes.forEach(function (v, i){
        addDestructor(v)
    });
    return this;
}
