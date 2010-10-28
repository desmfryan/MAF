/*
 *  mafHierarchyTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafHierarchy.h>
#include <mafSceneNode.h>
#include <mafVME.h>
#include <mafVisualPipe.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>
#include <iostream>
using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


/**
 Class name: testVisualPipeCustom
 This class implements a custom visual pipe.
 */
class  testVisualPipeCustom : public mafVisualPipe {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafVisualPipe);

public:
    /// Object constructor;
    testVisualPipeCustom(const mafString code_location = "") : mafVisualPipe(code_location) {};

    /// Object destructor.
    /* virtual */ ~testVisualPipeCustom() {};

    /// Initialize ans create the pipeline.
    /*virtual*/ void createPipe() {};

public slots:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1) {Q_UNUSED(t);};

private:

};

/**
 Class name: mafHierarchyTest
 This class implements the test suite for mafHierarchy.
 */

//! <title>
//mafHierarchy
//! </title>
//! <description>
//mafHierarchy provides basic API manage a tree of generic mafObjectBase.
//A scenegraph contains a tree of scenenodes which are defined as structs
//that contains a VME and a VisualPipe. It is also defined a tree iterator,
//that represents the current position inside the tree.
//! </description>
class mafHierarchyTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Hierarchy = mafNEW(mafCore::mafHierarchy);
        unbalancedTreeRandomCreation(3000); //Qt Creator gives problems in test machine with 10^5 nodes

        m_HierarchyToManage = mafNEW(mafCore::mafHierarchy);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        unbalancedTreeDestruction();
        mafDEL(m_Hierarchy);
        mafDEL(m_HierarchyToManage);
        mafEventBus::mafEventBusManager::instance()->shutdown();
    }

    /// mafSceneGraph allocation test case.
    void mafSceneGraphAllocationTest();
    /// benchmarking test, search in a vme added in the middle of tree generation
    void mafSceneGraphBenchmarkMiddleTest();

    /// benchmarking test, search in a vme added in the end of tree generation
    void mafSceneGraphBenchmarkEndTest();

    /// mafSceneGraph manage test case.
    void mafSceneGraphManageTest();


private:
    mafHierarchy *m_HierarchyToManage; ///< Test var.
    mafHierarchy *m_Hierarchy; ///< Test var.
    mafString m_CustomHashInTheMiddle; ///< hash to find that is located in the middle of the generation of the tree
    mafString m_CustomHashInTheEnd; ///< hash to find that is located in the end of the generation of the tree
    mafList<mafVME *> m_VMEList; ///< VME list which must be used to delete internal objects

    /// function for quick random creation of an unbalanced tree
    void unbalancedTreeRandomCreation(unsigned int numberOfElements);
    /// function for the destruction of the tree
    void unbalancedTreeDestruction();

};

void mafHierarchyTest::mafSceneGraphAllocationTest() {
    QVERIFY(m_Hierarchy != NULL);
}

void mafHierarchyTest::mafSceneGraphManageTest() {
    QVERIFY(m_HierarchyToManage != NULL);

    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");
    testVisualPipeCustom *pipeRoot = mafNEW(testVisualPipeCustom);

    mafSceneNode *sceneNodeRoot = new mafSceneNode(vmeRoot, pipeRoot, mafCodeLocation);
    m_HierarchyToManage->addHierarchyNode(sceneNodeRoot);

    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");
    testVisualPipeCustom *pipeChild0 = new testVisualPipeCustom();
    mafSceneNode *sceneNodeChild0 = new mafSceneNode(vmeChild0, pipeChild0, mafCodeLocation);
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild0);
    
    mafVME* vmeChild0Child0 = mafNEW(mafResources::mafVME);
    vmeChild0Child0->setObjectName("vmeChild0Child0");
    testVisualPipeCustom *pipeChild0Child0 = new testVisualPipeCustom();

    mafSceneNode *sceneNodeChild0Child0 = new mafSceneNode(vmeChild0Child0, pipeChild0Child0, mafCodeLocation);
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild0Child0);

    // Search the node by VME hash
    mafVisitorFindSceneNodeByVMEHash *v0 = new mafVisitorFindSceneNodeByVMEHash(sceneNodeChild0->vme()->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v0);
    mafSceneNode *n0 = v0->sceneNode();
    mafDEL(v0);
    m_HierarchyToManage->moveTreeIteratorToNode(n0);

    // Create a new child to add to the tree.
    mafVME* vmeChild1 = mafNEW(mafResources::mafVME);
    vmeChild1->setObjectName("vmeChild1");
    testVisualPipeCustom *pipeChild1 = new testVisualPipeCustom();
    mafSceneNode *sceneNodeChild1 = new mafSceneNode(vmeChild1, pipeChild1, mafCodeLocation);

    // Add the created scene node to the tree.
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild1);

    mafString information;
    m_HierarchyToManage->printInformation(information);
    mafMsgDebug() << information;

    // Search the node by VME hash
    mafVisitorFindSceneNodeByVMEHash *v1 = new mafVisitorFindSceneNodeByVMEHash(sceneNodeChild0->vme()->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v1);
    mafSceneNode *n1 = v1->sceneNode();
    mafDEL(v1);
    if(n1) {
        // Remove the found node tree.
        m_HierarchyToManage->removeHierarchyNode(n1);
        mafMsgDebug() << "Remove SceneNode";
    }
    information.clear();

    // Dump the tree hierarchy
    m_HierarchyToManage->printInformation(information);
    mafMsgDebug() << information;

    // Remove the allocated objects
    mafDEL(vmeChild1);
    mafDEL(vmeChild0Child0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);

    // Clear the tree.
    m_HierarchyToManage->clear();
}

void mafHierarchyTest::mafSceneGraphBenchmarkMiddleTest() {
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(m_CustomHashInTheMiddle, mafCodeLocation);
    QBENCHMARK {
        mafObjectRegistry::instance()->findObjectsThreaded(v);
    }
    mafDEL(v);
}

void mafHierarchyTest::mafSceneGraphBenchmarkEndTest() {
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(m_CustomHashInTheEnd, mafCodeLocation);
    QBENCHMARK {
        mafObjectRegistry::instance()->findObjectsThreaded(v);
    }
    mafDEL(v);
}

void mafHierarchyTest::unbalancedTreeRandomCreation(unsigned int numberOfElements) {
    REQUIRE(numberOfElements != 0);
    m_Hierarchy->clear();
    
    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");
    testVisualPipeCustom *pipeRoot = new testVisualPipeCustom();
    mafSceneNode *sceneNodeRoot = new mafSceneNode(vmeRoot, pipeRoot, mafCodeLocation);
    m_Hierarchy->addHierarchyNode(sceneNodeRoot);
    m_VMEList.push_back((mafResources::mafVME *)sceneNodeRoot->vme());

    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");
    testVisualPipeCustom *pipeChild0 = new testVisualPipeCustom();
    mafSceneNode *sceneNodeChild0 = new mafSceneNode(vmeChild0, pipeChild0, mafCodeLocation);
    m_Hierarchy->addHierarchyNode(sceneNodeChild0);
    m_VMEList.push_back((mafResources::mafVME *)sceneNodeChild0->vme());

    unsigned int count = 1;
    enum {
      SIBLING = 0,
      CHILD = 1,
    };
    int max = CHILD;
    int min = SIBLING;
    int randomNumber = 0;
    while(count != numberOfElements) {
        //generate random number from which decides where to insert node
        randomNumber = (int)(rand()/(RAND_MAX + 1.0) * (max + 1 - min) + min);
        //basing on random number determine if iterator must create a child or a sibling
        if(randomNumber == SIBLING) {
            m_Hierarchy->moveTreeIteratorToParent();
        } //else it will be a children

        mafVME* vmeChild = mafNEW(mafResources::mafVME);
        mafString name = "vmeChild";
        name.append(mafString::number(count));
        vmeChild->setObjectName(name);
        testVisualPipeCustom *pipeChild = new testVisualPipeCustom();
        mafSceneNode *sceneNode = new mafSceneNode(vmeChild, pipeChild);

        m_VMEList.push_back((mafResources::mafVME *)sceneNode->vme());

        if(count == numberOfElements / 2) {
            m_CustomHashInTheMiddle == vmeChild->objectHash();
        }

        if(count == numberOfElements-10) {
            m_CustomHashInTheEnd == vmeChild->objectHash();
        }

        m_Hierarchy->addHierarchyNode(sceneNode);
        count++;
    }
}

void mafHierarchyTest::unbalancedTreeDestruction() {
    mafVME *vme;
    foreach(vme, m_VMEList) {
        mafDEL(vme);
    }
    m_VMEList.clear();
    m_Hierarchy->clear();
}

MAF_REGISTER_TEST(mafHierarchyTest);
#include "mafHierarchyTest.moc"
