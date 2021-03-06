//
// Created by Evan Kirkiles on 7/24/18.
//

#include "titlebar.hpp"

// Builds the dark title bar
DarkTitleBar::DarkTitleBar(QWidget *parent, bool p_moreInfo, bool p_refresh, bool p_clear) :
        moreInfo(p_moreInfo), refresh(p_refresh), clear(p_clear), parentWidg(parent){

    // Build the title bar's necessary components
    title = new QLabel(parent->windowTitle(), this);
    if (((refresh && !clear) || (!refresh && clear)) && !moreInfo) {
        title->setStyleSheet("margin-right: 56px;");
    } else if (((refresh && !clear) || (!refresh && clear)) && moreInfo) {
        title->setStyleSheet("margin-right: 23px;");
    } else if (refresh && clear && !moreInfo) {
        title->setStyleSheet("margin-right: 89px;");
    }


    // Clickable buttons in top left
    closeWindow = new ClickableImage(24, 24, 2, file_paths::EXIT2_IMG, file_paths::EXIT_IMG, this);

    // Make sure the title bar always has a fixed height
    setFixedHeight(25);
    setObjectName("titlebar");

    // Uses a horizontal box layout obviously
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 1, 5, 1);
    mainLayout->addWidget(closeWindow);

    // If the refresh option is enabled, add a refresh button to the top left
    if (refresh) {
        // Clickable buttons in top left
        refreshButton = new ClickableImage(26, 26, 2, file_paths::REFRESHICON2_IMG, file_paths::REFRESHICON_IMG, this);
        mainLayout->addWidget(refreshButton);
    }
    // If the clear button is enabled, add a clear button to the top left
    if (clear) {
        // Clickable button in the top left
        clearButton = new ClickableImage(26, 26, 2, file_paths::CLEAR2_IMG, file_paths::CLEAR_IMG, this);
        mainLayout->addWidget(clearButton);
    }

    mainLayout->addStretch();
    mainLayout->addWidget(title);
    mainLayout->addStretch();

    // If the moreinfo option is enabled, add a moreinfo button to the top right
    if (moreInfo) {
        // Clickable buttons in top right
        moreInfoButton = new ClickableImage(26, 26, 2, file_paths::MOREINFO2_IMG, file_paths::MOREINFO_IMG, this);
        mainLayout->addWidget(moreInfoButton);
    }

    // Connect the pushbutton to the close function of the parent widget
    connect(closeWindow, SIGNAL(clicked()), parent, SLOT(close()));
}

// Override the mouse press event
void DarkTitleBar::mousePressEvent(QMouseEvent *event) {
    // When the user clicks with the left button, save the position of the click relative to window
    if (event->buttons() & Qt::LeftButton) {
        cursorPoint = event->globalPos() - parentWidg->geometry().topLeft();
        clickedOnButton = childAt(event->pos()) == closeWindow ||
                (moreInfo && childAt(event->pos()) == moreInfoButton) ||
                (refresh && childAt(event->pos()) == refreshButton);

        if (moreInfo) {
            if (childAt(event->pos()) == moreInfoButton) {
                showMoreInfo = true;
                emit showMIW();
            }
        }
        event->accept();
    }
}
// Override the mouse move event
void DarkTitleBar::mouseMoveEvent(QMouseEvent *event) {
    // When the user holds the left button down, move the window around with it
    if (event->buttons() & Qt::LeftButton && !clickedOnButton) {
        parentWidg->move(event->globalPos() - cursorPoint);
        event->accept();
    }
}
// Override the mouse release event
void DarkTitleBar::mouseReleaseEvent(QMouseEvent *event) {
    // If the moreinfowindow is being shown, then close it
    if (showMoreInfo) {
        showMoreInfo = false;
        emit hideMIW();
    }
}