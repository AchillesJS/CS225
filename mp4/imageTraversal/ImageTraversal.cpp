#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 * 
 * @param p1 First pixel
 * @param p2 Second pixel
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double hueDiff = fabs(p1.h - p2.h);
  double satDiff = p1.s - p2.s;
  double lumDiff = p1.l - p2.l;

  if (hueDiff > 180) { hueDiff = 360 - hueDiff; }
  hueDiff /= 360;

  return sqrt(hueDiff * hueDiff + satDiff * satDiff + lumDiff * lumDiff);
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  traversal_ = NULL;
  isComplete = true;
}

ImageTraversal::Iterator::Iterator(PNG png, Point startPoint, double tolerance, ImageTraversal* tra) {
  /** @todo [Part 1] */
  snapshot_ = png;
  cursor_ = startPoint;
  base_ = startPoint;
  threshold_ = tolerance;
  traversal_ = tra;
  isComplete = false;

  for (unsigned i = 0; i < snapshot_.width() * snapshot_.height(); i++) {
    explored_.push_back(false);
  }

  if (canVisit(cursor_)) {
    explored_[cursor_.x + cursor_.y * snapshot_.width()] = true;
    trace_.push_back(cursor_);
  } else {
    isComplete = true;
  }
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  Point right(cursor_.x + 1, cursor_.y);
  Point down(cursor_.x, cursor_.y + 1);
  Point left(cursor_.x - 1, cursor_.y);
  Point up(cursor_.x, cursor_.y - 1);

  if (canVisit(right)) traversal_->add(right);
  if (canVisit(down)) traversal_->add(down);
  if (canVisit(left)) traversal_->add(left);
  if (canVisit(up)) traversal_->add(up);

  if (traversal_->empty()) {
    setEnd(true);
    return *this;
  }

  Point next = traversal_->pop();
  while (explored_[next.x + next.y * snapshot_.width()]) {
    if (traversal_->empty()) {
      setEnd(true);
      return *this;
    }
    next = traversal_->pop();
  }

  cursor_ = next;
  explored_[cursor_.x + cursor_.y * snapshot_.width()] = true;
  trace_.push_back(cursor_);
  return *this;
}

bool ImageTraversal::Iterator::canVisit(const Point& point) {
  if (point.x >= snapshot_.width() || point.y >= snapshot_.height()) 
  return false;
  HSLAPixel origin = *(snapshot_.getPixel(base_.x, base_.y));
  HSLAPixel target = *(snapshot_.getPixel(point.x, point.y));
  return calculateDelta(origin, target) < threshold_;
}

void ImageTraversal::Iterator::setEnd(bool end){
  isComplete = end;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return cursor_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return (isComplete != other.isComplete);
}

