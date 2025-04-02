#pragma once
class Node
{
public: 
    enum Status {
    BH_INVALID,
    BH_SUCCESS,
    BH_FAILURE,
    BH_ABORTED,
    BH_RUNNING,
};

    virtual Status update() = 0;
    virtual void onInitialize() {}
    virtual void onTerminate(Status) {}

    Status tick() {
        if (m_eStatus != BH_RUNNING)
            onInitialize();
        m_eStatus = update();
        if (m_eStatus != BH_RUNNING)
            onTerminate(m_eStatus);
        return m_eStatus;
    }

    virtual ~Node() {}
protected:
    Status m_eStatus = BH_INVALID;
   
};

