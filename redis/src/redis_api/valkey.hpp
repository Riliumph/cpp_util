#include <sw/redis++/redis++.h>

namespace redis {

void
set_data(sw::redis::Redis& client);
void
set_data_with_expiration(sw::redis::Redis& client);
}
