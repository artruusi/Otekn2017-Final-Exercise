#include "agentwidget.h"


AgentWidget::AgentWidget(QWidget *parent) :
    QLabel(parent),
    id_(NO_ID),
    type_(NO_TYPE),
    hasAgent_(false),
    slot_location_(nullptr)
{
    setAcceptDrops(false);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void AgentWidget::setId(CardId id)
{
    id_ = id;
}

CardId AgentWidget::getId() const
{
    return  id_;
}

Type AgentWidget::getType() const
{
    return type_;
}

std::weak_ptr<Interface::Player> AgentWidget::getOwner() const
{
    return owner_;
}

std::shared_ptr<Interface::Location> AgentWidget::getSlotLocation() const
{
    return slot_location_;
}

bool AgentWidget::hasAgent() const
{
    return hasAgent_;
}

void AgentWidget::player_changed(std::weak_ptr<Interface::Player> next_player)
{
    current_player_ = next_player;
}

void AgentWidget::setAgent(bool b)
{
    hasAgent_ = b;
}

void AgentWidget::setType(Type type)
{
    type_ = type;
}

void AgentWidget::setOwner(std::weak_ptr<Interface::Player> new_owner)
{
    owner_ = new_owner;
}

void AgentWidget::setColour(QString new_colour)
{
    colour_ = new_colour;
    setStyleSheet("QLabel { color : " + colour_ + "; }");
}

void AgentWidget::setSlotLocation(std::shared_ptr<Interface::Location> location)
{
    slot_location_ = location;
}

void AgentWidget::mousePressEvent(QMouseEvent *event)
{
    if(type_ != "hand"){
        return;
    }

    const QPixmap* pixmap = this->pixmap();
    CardId source = id_;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << text() << *pixmap << colour_ << QPoint(event->pos() - pos()) << source;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap((*pixmap).scaled(110,160));
    drag->setHotSpot(event->pos());

    setVisible(false);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {

    }
    else {
        setVisible(true);
    }
}

void AgentWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
           if (event->source() == this) {
               event->setDropAction(Qt::MoveAction);
               event->accept();
           } else {
               event->acceptProposedAction();
           }
       } else {
           event->ignore();
       }
}

void AgentWidget::dragMoveEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
            if (event->source() == this) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            } else {
                event->acceptProposedAction();
            }
        } else {
            event->ignore();
        }
}

void AgentWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
            QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);

            QString agentName;
            QPixmap pixmap;
            QString colour;
            QPoint offset;
            CardId source;


            dataStream >> agentName >> pixmap >> colour >> offset >> source;

            if (source == "influ") {
                if (event->source() != this){
                    event->setDropAction(Qt::MoveAction);
                    event->accept();
                    emit influencePlaced(this);
                } else {
                    event->acceptProposedAction();
                }
            } else {

                setText(agentName);
                setPixmap(pixmap.scaled(77,122));
                setAgent(true);
                setId(source);
                setOwner(current_player_);
                setColour(colour);

                if (event->source() != this) {
                    event->setDropAction(Qt::MoveAction);
                    event->accept();
                    setAcceptDrops(false);
                    emit agentPlaced(source, this);
                } else {
                    event->acceptProposedAction();
                }
            }
        } else {
            event->ignore();
    }
}
