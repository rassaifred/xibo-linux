#include "Scheduler.hpp"

#include "control/IMainLayout.hpp"
#include "creators/MainDirector.hpp"
#include "utils/Utilities.hpp"
#include "utils/Logger.hpp"
#include "utils/Resources.hpp"
#include "constants.hpp"

#include <boost/property_tree/ptree.hpp>
#include <filesystem>

const size_t FIRST_ITEM_INDEX = 0;

void Scheduler::update(const Schedule::Result& schedule)
{
    m_layoutToPlayIndex = FIRST_ITEM_INDEX;
    m_layouts.clear();

    m_defaultLayout = schedule.defaultLayout;
    for(auto layout : schedule.layouts)
    {
        m_layouts.push_back(layout);
    }
}

std::unique_ptr<IMainLayout> Scheduler::nextLayout()
{
    auto layout = createLayout();

    layout->subscribe(EventType::DurationExpired, [=](const Event&){
        Log::debug("Got event layout expired");

        m_layoutToPlayIndex = getNextLayoutIndex();
        pushEvent(LayoutExpiredEvent{});
    });

    return layout;
}

std::unique_ptr<IMainLayout> Scheduler::createLayout()
{
    auto parsedXlfTree = Utils::parseXmlFromPath(getLayoutXlfPath());

    MainDirector director;
    return director.buildLayoutWithChildren(parsedXlfTree);
}

std::string Scheduler::getLayoutXlfPath()
{
    auto xlfFile = std::to_string(getNextLayoutId()) + ".xlf";
    return Resources::directory() / xlfFile;
}

int Scheduler::getNextLayoutId()
{
    if(!m_layouts.empty())
    {
        auto&& layout = m_layouts[m_layoutToPlayIndex];
        return layout.id;
    }
    else
    {
        return m_defaultLayout.id;
    }
}

size_t Scheduler::getNextLayoutIndex()
{
    size_t nextLayoutToPlayIndex = m_layoutToPlayIndex + 1;

    if(nextLayoutToPlayIndex >= m_layouts.size())
        return FIRST_ITEM_INDEX;

    return nextLayoutToPlayIndex;
}

