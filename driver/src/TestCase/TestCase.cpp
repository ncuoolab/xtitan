/*
Copyright (c) 2011 NCU OOlab <ncuoolab@gmail.com>
*/
#include "TestCase_p.hpp"


using xtitan::TestCase;


TestCase::Private::Private():
activated( true ),
creator(),
description(),
html(),
level(),
mapping(),
mtime(),
name(),
options(),
script() {
}

TestCase::TestCase(): p_( new Private ) {
}

const QString & TestCase::getCreator() const {
	return this->p_->creator;
}

const QString & TestCase::getDescription() const {
	return this->p_->description;
}

const QString & TestCase::getHTML() const {
	return this->p_->html;
}

const std::map< QString, QUrl > & TestCase::getImageMapping() const {
	return this->p_->mapping;
}

int TestCase::getLevel() const {
	return this->p_->level;
}

const QDateTime & TestCase::getModifyTime() const {
	return this->p_->mtime;
}

const QString & TestCase::getName() const {
	return this->p_->name;
}

const QString & TestCase::getProgramOptions() const {
	return this->p_->options;
}

const QString & TestCase::getScript() const {
	return this->p_->script;
}

bool TestCase::isActivated() const {
	return this->p_->activated;
}

void TestCase::addImageMapping( const QString & name, const QUrl & uri ) {
	std::map< QString, QUrl >::iterator it = this->p_->mapping.find( name );
	if( it == this->p_->mapping.end() ) {
		this->p_->mapping.insert( std::make_pair( name, uri ) );
	} else {
		it->second = uri;
	}
}

void TestCase::setActivated( bool activated ) {
	this->p_->activated = activated;
}

void TestCase::setCreator( const QString & creator ) {
	this->p_->creator = creator;
}

void TestCase::setDescription( const QString & description ) {
	this->p_->description = description;
}

void TestCase::setHTML( const QString & html ) {
	this->p_->html = html;
}

void TestCase::setImageMapping( const std::map< QString, QUrl > & mapping ) {
	this->p_->mapping = mapping;
}

void TestCase::setLevel( int level ) {
	this->p_->level = level;
}

void TestCase::setModifyTime( const QDateTime & timestamp ) {
	this->p_->mtime = timestamp;
}

void TestCase::setName( const QString & name ) {
	this->p_->name = name;
}

void TestCase::setProgramOptions( const QString & options ) {
	this->p_->options = options;
}

void TestCase::setScript( const QString & script ) {
	this->p_->script = script;
}
