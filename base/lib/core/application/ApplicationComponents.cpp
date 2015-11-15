#include "ApplicationComponents.hpp"
namespace iscore
{
iscore::SerializableCommand* ApplicationComponents::instantiateUndoCommand(
        const CommandParentFactoryKey& parent_name,
        const CommandFactoryKey& name,
        const QByteArray& data)
{
    auto it = m_data.commands.find(parent_name);
    if(it != m_data.commands.end())
    {
        auto it2 = it->second.find(name);
        if(it2 != it->second.end())
        {
            return (*it2->second)(data);
        }
    }

#if defined(ISCORE_DEBUG)
    qDebug() << "ALERT: Command"
             << parent_name
             << "::"
             << name
             << "could not be instantiated.";
    ISCORE_ABORT;
#else
    throw MissingCommandException(parent_name, name);
#endif
    return nullptr;
}
}
