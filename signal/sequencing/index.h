#include "../constants.h"
#include "../node.h"

namespace libsignal
{

	class Index : public Node
	{

	public:

		Index(PropertyRef list = {}, NodeRef index = 0);

		virtual void next(sample **out, int num_frames);

		PropertyRef list;
		NodeRef index;
	};

	REGISTER(Index, "index");

}
