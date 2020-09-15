#ifndef INPUT_H
#define INPUT_H

#define KEY_COUNT 256

typedef unsigned int KeyCode;

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Init();

	void KeyDown(KeyCode);
	void KeyUp(KeyCode);
	void ButtonDown(KeyCode);
	void ButtonUp(KeyCode);
	void UpdateMousePos(int x, int y);

	bool IsKeyDown(KeyCode) const;
	bool IsMouseDown(KeyCode) const;
	int  MouseDeltaX() const;
	int  MouseDeltaY() const;

private:
	bool m_keys[KEY_COUNT];
	bool m_buttons[17];
	int m_mouseDelta[2];
	int m_mousePos[2];
};


#endif // !INPUT_H




