#include <sw/redis++/redis++.h>

namespace redis {

void
input_list_data(sw::redis::Redis& client);

void
input_capped_list_data(sw::redis::Redis& client);

}
