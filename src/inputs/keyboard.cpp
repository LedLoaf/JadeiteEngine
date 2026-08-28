#include "keyboard.hpp"
#include "keys.hpp"

namespace jadeite
{
Keyboard::Keyboard()
	: m_mapButtons{
	{KEY_BACKSPACE, Button{}},   {KEY_TAB, Button{}},        {KEY_CLEAR, Button{}},
	{KEY_RETURN, Button{}},      {KEY_PAUSE, Button{}},      {KEY_ESCAPE, Button{}},
	{KEY_SPACE, Button{}},       {KEY_EXCLAIM, Button{}},    {KEY_QUOTEDBL, Button{}},
	{KEY_HASH, Button{}},        {KEY_DOLLAR, Button{}},     {KEY_AMPERSAND, Button{}},
	{KEY_QUOTE, Button{}},       {KEY_LEFTPAREN, Button{}},  {KEY_RIGHTPAREN, Button{}},
	{KEY_ASTERISK, Button{}},    {KEY_PLUS, Button{}},       {KEY_COMMA, Button{}},
	{KEY_PERIOD, Button{}},      {KEY_SLASH, Button{}},      {KEY_0, Button{}},
	{KEY_2, Button{}},           {KEY_3, Button{}},          {KEY_4, Button{}},
	{KEY_5, Button{}},           {KEY_6, Button{}},          {KEY_7, Button{}},
	{KEY_8, Button{}},           {KEY_9, Button{}},          {KEY_COLON, Button{}},
    {KEY_SEMICOLON, Button{}},   {KEY_LESS, Button{}},       {KEY_EQUALS, Button{}},
    {KEY_GREATER, Button{}},     {KEY_QUESTION, Button{}},   {KEY_AT, Button{}},
    {KEY_LEFTBRACKET, Button{}}, {KEY_BACKSLASH, Button{}},  {KEY_RIGHTBRACKET, Button{}},
    {KEY_CARET, Button{}},       {KEY_UNDERSCORE, Button{}}, {KEY_A, Button{}},
    {KEY_B, Button{}},           {KEY_C, Button{}},          {KEY_D, Button{}},
    {KEY_E, Button{}},           {KEY_F, Button{}},          {KEY_G, Button{}},
    {KEY_H, Button{}},           {KEY_I, Button{}},          {KEY_J, Button{}},
    {KEY_K, Button{}},           {KEY_L, Button{}},          {KEY_M, Button{}},
    {KEY_N, Button{}},           {KEY_O, Button{}},          {KEY_P, Button{}},
    {KEY_Q, Button{}},           {KEY_R, Button{}},          {KEY_S, Button{}},
    {KEY_T, Button{}},           {KEY_U, Button{}},          {KEY_V, Button{}},
    {KEY_W, Button{}},           {KEY_X, Button{}},          {KEY_Y, Button{}},
    {KEY_Z, Button{}},           {KEY_DELETE, Button{}},     {KEY_CAPSLOCK, Button{}},
    {KEY_F1, Button{}},          {KEY_F2, Button{}},         {KEY_F3, Button{}},
    {KEY_F4, Button{}},          {KEY_F5, Button{}},         {KEY_F6, Button{}},
    {KEY_F7, Button{}},          {KEY_F8, Button{}},         {KEY_F9, Button{}},
    {KEY_F10, Button{}},         {KEY_F11, Button{}},        {KEY_F12, Button{}},
    {KEY_SCROLLOCK, Button{}},   {KEY_INSERT, Button{}},     {KEY_HOME, Button{}},
    {KEY_PAGEUP, Button{}},      {KEY_PAGEDOWN, Button{}},   {KEY_END, Button{}},
    {KEY_RIGHT, Button{}},       {KEY_LEFT, Button{}},       {KEY_DOWN, Button{}},
    {KEY_UP, Button{}},          {KEY_NUMLOCK, Button{}},    {KEY_KP_DIVIDE, Button{}},
    {KEY_KP_MULTIPLY, Button{}}, {KEY_KP_MINUS, Button{}},   {KEY_KP_PLUS, Button{}},
    {KEY_KP_ENTER, Button{}},    {KEY_KP1, Button{}},        {KEY_KP2, Button{}},
    {KEY_KP3, Button{}},         {KEY_KP4, Button{}},        {KEY_KP5, Button{}},
    {KEY_KP6, Button{}},         {KEY_KP7, Button{}},        {KEY_KP8, Button{}},
    {KEY_KP9, Button{}},         {KEY_KP0, Button{}},        {KEY_KP_PERIOD, Button{}},
    {KEY_LCTRL, Button{}},       {KEY_LSHIFT, Button{}},     {KEY_LALT, Button{}},
    {KEY_RCTRL, Button{}},       {KEY_RSHIFT, Button{}},     {KEY_RALT, Button{}}}
{
	
}

Keyboard::~Keyboard() = default;

void Keyboard::Update()
{
	for (auto& [_, button] : m_mapButtons)
	{
		button.Reset();
	}
}

void Keyboard::OnKeyPressed(int key)
{
	if (key == KEY_UNKNOWN)
		return;
	
	auto keyItr = m_mapButtons.find(key);
	if (keyItr == m_mapButtons.end())
	{
		return;
	}
	keyItr->second.Update(true);
}

void Keyboard::OnKeyReleased(int key)
{
	if (key == KEY_UNKNOWN)
		return;
	
	auto keyItr = m_mapButtons.find(key);
	if (keyItr == m_mapButtons.end())
	{
		return;
	}
	keyItr->second.Update(false);
}

const bool Keyboard::IsKeyPressed(int key) const
{
	if (key == KEY_UNKNOWN)
		return false;
	
	auto keyItr = m_mapButtons.find(key);
	if (keyItr == m_mapButtons.end())
	{
		return false;
	}
	
	return keyItr->second.bIsPressed;
}

const bool Keyboard::IsKeyJustPressed(int key) const
{
	if (key == KEY_UNKNOWN)
		return false;
	
	auto keyItr = m_mapButtons.find(key);
	if (keyItr == m_mapButtons.end())
	{
		return false;
	}
	
	return keyItr->second.bJustPressed;
}

const bool Keyboard::IsKeyJustReleased(int key) const
{
	if (key == KEY_UNKNOWN)
		return false;
	
	auto keyItr = m_mapButtons.find(key);
	if (keyItr == m_mapButtons.end())
	{
		return false;
	}
	
	return keyItr->second.bJustReleased;
}

const bool Keyboard::IsAnyKeyPressed() const
{
	return std::ranges::any_of(
		m_mapButtons, [](const auto& pair) {  return pair.second.bIsPressed; }
	);
}

void Keyboard::CreateLuaBind(sol::state& lua, Keyboard& keyboard)
{
    // ==================================================================
    // Register Typewriter Keys
    // ==================================================================
    lua.set("KEY_A", KEY_A);
    lua.set("KEY_B", KEY_B);
    lua.set("KEY_C", KEY_C);
    lua.set("KEY_D", KEY_D);
    lua.set("KEY_E", KEY_E);
    lua.set("KEY_F", KEY_F);
    lua.set("KEY_G", KEY_G);
    lua.set("KEY_H", KEY_H);
    lua.set("KEY_I", KEY_I);
    lua.set("KEY_J", KEY_J);
    lua.set("KEY_K", KEY_K);
    lua.set("KEY_L", KEY_L);
    lua.set("KEY_M", KEY_M);
    lua.set("KEY_N", KEY_N);
    lua.set("KEY_O", KEY_O);
    lua.set("KEY_P", KEY_P);
    lua.set("KEY_Q", KEY_Q);
    lua.set("KEY_R", KEY_R);
    lua.set("KEY_S", KEY_S);
    lua.set("KEY_T", KEY_T);
    lua.set("KEY_U", KEY_U);
    lua.set("KEY_V", KEY_V);
    lua.set("KEY_W", KEY_W);
    lua.set("KEY_X", KEY_X);
    lua.set("KEY_Y", KEY_Y);
    lua.set("KEY_Z", KEY_Z);

    lua.set("KEY_0", KEY_0);
    lua.set("KEY_1", KEY_1);
    lua.set("KEY_2", KEY_2);
    lua.set("KEY_3", KEY_3);
    lua.set("KEY_4", KEY_4);
    lua.set("KEY_5", KEY_5);
    lua.set("KEY_6", KEY_6);
    lua.set("KEY_7", KEY_7);
    lua.set("KEY_8", KEY_8);
    lua.set("KEY_9", KEY_9);

    lua.set("KEY_ENTER", KEY_RETURN);
    lua.set("KEY_BACKSPACE", KEY_BACKSPACE);
    lua.set("KEY_ESC", KEY_ESCAPE);
    lua.set("KEY_SPACE", KEY_SPACE);
    lua.set("KEY_LCTRL", KEY_LCTRL);
    lua.set("KEY_RCTRL", KEY_RCTRL);
    lua.set("KEY_LALT", KEY_LALT);
    lua.set("KEY_RALT", KEY_RALT);
    lua.set("KEY_LSHIFT", KEY_LSHIFT);
    lua.set("KEY_RSHIFT", KEY_RSHIFT);

    // ==================================================================
    //  Register Punctuation Keys
    // ==================================================================
    lua.set("KEY_COLON", KEY_COLON);
    lua.set("KEY_SEMICOLON", KEY_SEMICOLON);
    lua.set("KEY_QUOTE", KEY_QUOTE);
    lua.set("KEY_BACKQUOTE", KEY_BACKQUOTE);
    lua.set("KEY_CARET", KEY_CARET);
    lua.set("KEY_UNDERSCORE", KEY_UNDERSCORE);
    lua.set("KEY_RIGHTBRACKET", KEY_RIGHTBRACKET);
    lua.set("KEY_LEFTBRACKET", KEY_LEFTBRACKET);
    lua.set("KEY_SLASH", KEY_SLASH);
    lua.set("KEY_ASTERISK", KEY_ASTERISK);
    lua.set("KEY_LEFTPAREN", KEY_LEFTPAREN);
    lua.set("KEY_RIGHTPAREN", KEY_RIGHTPAREN);
    lua.set("KEY_QUESTION", KEY_QUESTION);
    lua.set("KEY_AMPERSAND", KEY_AMPERSAND);
    lua.set("KEY_DOLLAR", KEY_DOLLAR);
    lua.set("KEY_EXCLAIM", KEY_EXCLAIM);
    lua.set("KEY_BACKSLASH", KEY_BACKSLASH);

    // ==================================================================
    //  Register Function Keys
    // ==================================================================
    lua.set("KEY_F1", KEY_F1);
    lua.set("KEY_F2", KEY_F2);
    lua.set("KEY_F3", KEY_F3);
    lua.set("KEY_F4", KEY_F4);
    lua.set("KEY_F5", KEY_F5);
    lua.set("KEY_F6", KEY_F6);
    lua.set("KEY_F7", KEY_F7);
    lua.set("KEY_F8", KEY_F8);
    lua.set("KEY_F9", KEY_F9);
    lua.set("KEY_F10", KEY_F10);
    lua.set("KEY_F11", KEY_F11);
    lua.set("KEY_F12", KEY_F12);

    // ==================================================================
    // Register Cursor Control Keys
    // ==================================================================
    lua.set("KEY_UP", KEY_UP);
    lua.set("KEY_RIGHT", KEY_RIGHT);
    lua.set("KEY_DOWN", KEY_DOWN);
    lua.set("KEY_LEFT", KEY_LEFT);

    // ==================================================================
    // Register Numeric Keypad Keys
    // ==================================================================
    lua.set("KP_KEY_0", KEY_KP0);
    lua.set("KP_KEY_1", KEY_KP1);
    lua.set("KP_KEY_2", KEY_KP2);
    lua.set("KP_KEY_3", KEY_KP3);
    lua.set("KP_KEY_4", KEY_KP4);
    lua.set("KP_KEY_5", KEY_KP5);
    lua.set("KP_KEY_6", KEY_KP6);
    lua.set("KP_KEY_7", KEY_KP7);
    lua.set("KP_KEY_8", KEY_KP8);
    lua.set("KP_KEY_9", KEY_KP9);

    lua.set("KP_KEY_DIVIDE", KEY_KP_DIVIDE);
    lua.set("KP_KEY_MULTIPLY", KEY_KP_MULTIPLY);
    lua.set("KP_KEY_MINUS", KEY_KP_MINUS);
    lua.set("KP_KEY_PLUS", KEY_KP_PLUS);
    lua.set("KP_KEY_ENTER", KEY_KP_ENTER);
    lua.set("KP_KEY_PERIOD", KEY_KP_PERIOD);

    lua.set("KEY_NUM_LOCK", KEY_NUMLOCK);

	lua.new_usertype<Keyboard>(
		"Keyboard",
		sol::no_constructor,
		"justPressed", [&](int key) { return keyboard.IsKeyJustPressed( key ); },
		"justReleased", [&](int key) { return keyboard.IsKeyJustReleased( key ); },
		"pressed", [&](int key) { return keyboard.IsKeyPressed( key ); },
		"anyKeyPressed", [&] { return keyboard.IsAnyKeyPressed(); }
	);
}

} // jadeite
