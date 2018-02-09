#include "agent.h"
#include "location.h"
#include "stateexception.h"

#include <QSignalSpy>
#include <QtTest>

class TestAgent : public QObject
{
    Q_OBJECT

public:
    TestAgent();

private Q_SLOTS:
    void agentInitTest();
    void legalConnectionsTest();
    void illegalModifyConnectionsTest();
    void illegalConnectionsInvariantTest();
};

TestAgent::TestAgent()
{
}

void TestAgent::agentInitTest()
{
    unsigned short example_base_connections = 0;
    Agent::Agent example_agent("ExampleId", "ExampleName", "ExampleTitle");
    QVERIFY2((example_base_connections == example_agent.connections()), "Base_connections test");
}

void TestAgent::legalConnectionsTest()
{
    Agent::Agent example_agent("ExampleId", "ExampleName", "ExampleTitle");
    example_agent.setConnections(5);
    QVERIFY2(example_agent.connections()==5, "Legal connections.");
}

void TestAgent::illegalModifyConnectionsTest()
{
    Agent::Agent example_agent("ExampleId", "ExampleName", "ExampleTitle");
    QVERIFY_EXCEPTION_THROWN(example_agent.modifyConnections(-5), Interface::StateException);
}

void TestAgent::illegalConnectionsInvariantTest()
{
    Agent::Agent example_agent("ExampleId", "ExampleName", "ExampleTitle");

    std::weak_ptr<Interface::Location> weak_location_ptr;
    auto location_ptr = std::make_shared<Interface::Location>(0, "TEST");
    weak_location_ptr = location_ptr;

    example_agent.setPlacement(weak_location_ptr);
    QVERIFY_EXCEPTION_THROWN(example_agent.modifyConnections(1), Interface::StateException);
}

QTEST_APPLESS_MAIN(TestAgent)

#include "tst_testagent.moc"
