
function addProperty(type, name, options, node, save){

    var isInline     = false
    var isNotifiable = false
    var isReference  = false
    var writeGetter  = false
    var writeSetter  = false
    var isQProperty  = false
    if (typeof options !== 'undefined'){
        isInline     = options.indexOf('i') !== -1;
        isNotifiable = options.indexOf('n') !== -1;
        isReference  = options.indexOf('r') !== -1;
        writeGetter  = options.indexOf('g') !== -1;
        writeSetter  = options.indexOf('s') !== -1;
        isQProperty  = options.indexOf('q') !== -1;
    }

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

    function capitaliseFirstLetter(str){
        return str.charAt(0).toUpperCase() + str.slice(1);
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

    var classNode = findNode()
    if ( classNode === null )
        throw new Error("Cannot find specified class.");

    var cppSourceNamespace = ''
    var cppSourceLocation  = null
    if ( !isInline ){
        var namespaceAndPosition = getSourceNamespaceAndPosition(classNode)
        if ( namespaceAndPosition === null ){
            isInline = true
        } else {
            cppSourceNamespace = namespaceAndPosition.cppBreadCrumbs
            cppSourceLocation  = namespaceAndPosition.node
        }
    }

    var member = '    ' + type + ' m_' + name + ';\n';

    var methodDecl = ''
    if ( writeGetter ){
        methodDecl +=
            '    ' + (isReference ? 'const ' : '') + type +
            (isReference ? '& '   : '') + name  +
            '() const;\n'
    }

    if ( writeGetter ){
        methodDecl +=
            '    void set' + capitaliseFirstLetter(name) + '(' +
            (isReference ? 'const ' : '') +
            type +
            (isReference ? '& ' : ' ') +
            name + ');\n';
    }

    var methodImpl = ''

    if ( writeGetter ){
        methodImpl +=
            '\n' + (isInline ? 'inline ' : '') +
            (isReference ? 'const ' : '') + type +
            (isReference ? '& ' : ' ') +
            cppSourceNamespace +
            classNode.identifier() + '::' + name +
            '() const{' + '\n    return m_' + name + ';\n}\n'
    }

    if ( writeSetter ){
        methodImpl +=
                '\n' + (isInline ? 'inline ' : '') + 'void ' +
                cppSourceNamespace +
                classNode.identifier() + '::set' + capitaliseFirstLetter(name) +
                '(' + (isReference ? 'const ' : '') + type + (isReference ? '& ' : '') +
                name + '){\n    ';

        if ( isNotifiable ){
            methodImpl +=
                    'if (m_' + name  + ' != ' + name + '){\n' +
                    '        m_' + name + ' = ' + name + ';\n' +
                    '        emit ' + name + 'Changed();\n' +
                    '    }\n';
        } else {
            methodImpl +=
                    '    m_' + propertyDeclaration + ' = ' + propertyDeclaration + ';\n';
        }
        methodImpl += '}\n';
    }

    var privateAccess = classNode.firstChild('private', 'access');
    if ( privateAccess !== null ){
        var nextPrivateMember = privateAccess;

        while ( nextPrivateMember !== null ){
            var afterNextPrivateMember = nextPrivateMember.next();
            if ( afterNextPrivateMember !== null ){
                if ( afterNextPrivateMember.typeName() === 'access' ){
                    nextPrivateMember.afterln(member);
                    break;
                }
            } else {
                nextPrivateMember.afterln(member);
                break;
            }
            nextPrivateMember = nextPrivateMember.next();
        }
    } else {
        classNode.append('private:\n' + member);
    }

    if ( methodDecl !== '' ){
        var publicAccess = classNode.firstChild('public', 'access');
        if ( publicAccess !== null ){

            var nextPublicMember = publicAccess.next();
            while ( nextPublicMember !== null ){
                var afterNextPublicMember = nextPublicMember.next();
                if ( afterNextPublicMember !== null ){
                    if ( afterNextPublicMember.typeName() === 'access' ){
                        nextPublicMember.afterln(methodDecl);
                        break;
                    }
                } else {
                    nextPublicMember.afterln(methodDecl);
                    break;
                }
                nextPublicMember = nextPublicMember.next();
            }

        } else {
            classNode.append('\npublic:\n' + methodDecl);
        }
    }

    if ( methodImpl !== '' ){
        if ( isInline )
            classNode.afterln(methodImpl)
        else
            cppSourceLocation.append(methodImpl)
    }

    if (typeof save !== 'undefined' ? save : true)
        codeBase.save();
}

NodeCollection.registerPlugin({
    'name' : 'addProperty(type, name, flags)',
    'usage' : 'addProperty("type", "name", "inrgsq")',
    'description' :
        'Adds a property (field, setter, getter) to the given class within the hierarchy.\n' +
        'Params:\n' +
            '\ttype <string> : Type of the property.\n' +
            '\tname <string> : Name of the property.\n' +
            '\tflags <string> : Property flags : \n' +
                '\t\t\'g\' : Create getter.\n' +
                '\t\t\'s\' : Create setter.\n' +
                '\t\t\'i\' : Use inline getter and setter.\n' +
                '\t\t\'r\' : Use reference for setters and getters.\n' +
                '\t\t\'q\' : Add Q_PROPERTY for QObject classes.\n' +
                '\t\t\'n\' : Added notify signal if property changes.'
}).prototype.addProperty = function(type, name, options){
    this.nodes.forEach(function (v, i){
        addProperty(type, name, options, v, false)
    });
    codeBase.save()
}
