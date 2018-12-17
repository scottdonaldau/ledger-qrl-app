/*******************************************************************************
*   (c) 2018 ZondaX GmbH
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

#include <shash.h>
#include "qrl_types.h"

#define PTR_DIST(p2, p1) (int16_t)(((int8_t *)(p2)) - ((int8_t *)(p1)))

int16_t get_qrltx_size(const qrltx_t *tx_p) {
    if (tx_p->subitem_count == 0) {
        return -1;
    }

    uint16_t req_size = 1;
    // validate sizes
    switch (tx_p->type) {
        case QRLTX_TX: {
            if (tx_p->subitem_count > QRLTX_SUBITEM_MAX) {
                return -1;
            }

            const int16_t delta = PTR_DIST(&tx_p->tx.dst, tx_p);
            req_size = delta + sizeof(qrltx_addr_block) * tx_p->subitem_count;
            break;
        }
#ifdef TXTOKEN_ENABLED
        case QRLTX_TXTOKEN: {
            if (tx_p->subitem_count > QRLTX_SUBITEM_MAX) {
                return -1;
            }

            const int16_t delta = PTR_DIST(&tx_p->txtoken.dst, tx_p);
            req_size = delta + sizeof(qrltx_addr_block) * tx_p->subitem_count;
            break;
        }
#endif
        case QRLTX_SLAVE: {
            if (tx_p->subitem_count > QRLTX_SUBITEM_MAX) {
                return -1;
            }

            const int16_t delta = PTR_DIST(&tx_p->slave.slaves, tx_p);
            req_size = delta + sizeof(qrltx_addr_block) * tx_p->subitem_count;
            break;
        }
        case QRLTX_MESSAGE: {
            if (tx_p->subitem_count > QRLTX_MESSAGE_SUBITEM_MAX) {
                return -1;
            }

            const int16_t delta = PTR_DIST(&tx_p->msg.message, tx_p);
            req_size = delta + sizeof(uint8_t) * tx_p->subitem_count;
            break;
        }
        default:
            break;
    }
    return req_size;
}

int8_t get_qrltx_hash(const qrltx_t *tx_p, uint8_t hash[32]) {
    int16_t in_len = get_qrltx_size(tx_p);

    if (in_len <= 0)
        return -1;

    uint8_t *p = ((uint8_t *) tx_p);

    // skip metadata and source address
    p += 2 + 39;
    in_len -= 2 + 39;

    __sha256(hash, p, (uint16_t) in_len);

    return 0;
}
