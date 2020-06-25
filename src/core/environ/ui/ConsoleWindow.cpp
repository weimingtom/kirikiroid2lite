#include "ConsoleWindow.h"
#include "cocos2d/MainScene.h"
#include <pthread.h>

using namespace cocos2d;

TVPConsoleWindow::TVPConsoleWindow() {

}

TVPConsoleWindow* TVPConsoleWindow::create(int fontSize, cocos2d::Node *parent) {
	TVPConsoleWindow *ret = new TVPConsoleWindow;
	ret->init();
	ret->setAnchorPoint(Vec2::ZERO);
	ret->setFontSize(fontSize);
	ret->autorelease();
	if (parent) {
		ret->setContentSize(parent->getContentSize());
	}
	return ret;
}

extern pthread_t *TVPMainThreadID;
void TVPConsoleWindow::addLine(const ttstr &line, cocos2d::Color3B clr) {
#if 0
	assert(TVPMainThreadID == boost::this_thread::get_id());
#else
	assert(pthread_equal(*TVPMainThreadID, pthread_self()));
#endif
	if (_queuedLines.size() > _maxQueueSize) {
		_queuedLines.pop_front();
	}
	_queuedLines.push_back(std::pair<ttstr, cocos2d::Color3B>(line, clr));
}


void TVPConsoleWindow::setFontSize(float size) {
	_fontSize = size;
	_maxQueueSize = getContentSize().height / size + 2;
}

void TVPConsoleWindow::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) {
	if (!_queuedLines.empty()) {
		float height = 0;
		std::vector<cocos2d::Label* > _queuedLabels;
		//for (std::pair<ttstr, cocos2d::Color3B> &line : _queuedLines) {
		for (auto p_line = _queuedLines.begin(); p_line != _queuedLines.end(); ++p_line)
		{
			auto& line = *p_line;
			cocos2d::Label* label;
			if (_unusedLabels.empty()) {
				Size dim(getContentSize());
				dim.height = 0;
				label = cocos2d::Label::createWithTTF("", "DroidSansFallback.ttf", _fontSize, dim);
				label->setAnchorPoint(Vec2::ZERO);
				addChild(label);
			} else {
				label = _unusedLabels.back();
				_unusedLabels.pop_back();
				label->setVisible(true);
			}
			label->setColor(line.second);
			ttstr t = line.first.length() > 200 ? line.first.SubString(0, 200) : line.first;
			label->setString(t.AsStdString());
			label->setPosition(0, height);
			height += label->getContentSize().height;
			_queuedLabels.push_back(label);
		}
		Size size = getContentSize();
		// remove out of range text
		while (!_dispLabels.empty()) {
			cocos2d::Label* label = _dispLabels.front();
			const Vec2 &pos = label->getPosition();
			if (pos.y + height > size.height) {
				label->setVisible(false);
				_unusedLabels.push_back(label);
				_dispLabels.pop_front();
			} else {
				break;
			}
		}

		// adjust position
		//for (cocos2d::Label* label : _dispLabels) {
		for (auto p_label = _dispLabels.begin(); p_label != _dispLabels.end(); ++p_label)
		{
			auto& label = *p_label;
			Vec2 pos = label->getPosition();
			pos.y += height;
			label->setPosition(pos);
		}

		//for (cocos2d::Label* label : _queuedLabels) {
		for (auto p_label = _queuedLabels.begin(); p_label != _queuedLabels.end(); ++p_label)
		{
			auto& label = *p_label;
			_dispLabels.push_back(label);
		}
		_queuedLabels.clear();
		_queuedLines.clear();
	}
	cocos2d::Node::visit(renderer, parentTransform, parentFlags);
}
