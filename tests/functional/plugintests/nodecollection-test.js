CSATest.describe('NodeCollection', function(test){

    test.scenario('node navigation', function(){
        var file = codebase.parseFile('coretests/nodecollection-test.in')
        assert(file !== null)
        assert(nodes('*Test').size() === 3)
        var classNode = nodes('*Test', 'class')
        assert(classNode.size() === 1)
        assert(classNode.children().size() === 3)
        assert(classNode.children().nodes[0].identifier() === 'public')
        assert(classNode.children().nodes[1].identifier() === 'intTest')
        assert(classNode.children().nodes[0].astParent().identifier() === 'NodeCollectionTest')
        assert(nodes(file).find('*Test').size() === 3)
        assert(nodes([file]).find('*', 'field').size() === 2)
    })

    test.scenario('append node', function(){
        var file = codebase.parseFile('coretests/nodecollection-test.in')
        assert(file !== null)
        assert(nodes('*Test').size() === 3)
        var ns = nodes('*Test', 'class')
        assert(ns.size() === 1)
        ns.append(nodes('intTest'))
        assert(ns.size() === 2)
        ns.append(nodes('stringTest').nodes[0])
        assert(ns.size() === 3)
    })

    test.scenario('node removal', function(){
        backup('coretests/nodecollection-test.in')
        var file = codebase.parseFile('coretests/nodecollection-test.in')
        assert(file !== null)
        nodes('intTest').remove()
        codebase.save()
        assert(nodes('intTest').size() === 0)
        restore()
    })

    test.scenario('extending functionality', function(){
        NodeCollection.extend({
            'getIdentifiers'  : function(){
                var identifiers = new Array
                this.each(function(v, i){
                    identifiers.push(v.identifier())
                })
                return identifiers;
            }
        })
        var file = codebase.parseFile('coretests/nodecollection-test.in')
        assert(file !== null)
        assert(nodes('*Test').size() === 3)
        var identifiers = nodes('*Test').getIdentifiers()
        assert(identifiers[0] === 'NodeCollectionTest')
        assert(identifiers[1] === 'intTest')
        assert(identifiers[2] === 'stringTest')
    })

    test.scenario('selected node', function(){
        var file = codebase.parseFile('coretests/nodecollection-test.in')
        assert(file !== null)
        var fieldNode = nodes('*intTest').nodes[0]
        assert(selected().size() === 0)
        NodeCollection.selectedPath = fieldNode.breadcrumbs()
        assert(selected().size() === 1)
        assert(selected().nodes[0].identifier() === 'intTest')

        NodeCollection.selectedPath = codebase.nodeAt(file.identifier(), 3, 15)
        assert(selected().nodes[0].identifier() === 'intTest')

        NodeCollection.selectedPath = codebase.nodeAt(file.identifier(), 45)
        assert(selected().nodes[0].identifier() === 'intTest')

        NodeCollection.selectedPath = codebase.nodeAt(file.identifier(), 10)
        assert(selected().nodes[0].identifier() === 'NodeCollectionTest')
    })

})
