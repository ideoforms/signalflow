#include "../node.h"

#ifdef __APPLE__

namespace libsignal
{
	class MouseX : public Node
	{
	public:
		MouseX();
		virtual void process(sample **out, int num_frames);
	};

	class MouseY : public Node
	{
	public:
		MouseY();
		virtual void process(sample **out, int num_frames);
	};

	REGISTER(MouseX, "mousex");
	REGISTER(MouseY, "mousey");
}

#endif /* __APPLE__ */
