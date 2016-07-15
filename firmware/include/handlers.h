
#ifndef HANDLERS_H
#define HANDLERS_H

#include "em_usb.h"

//Check macros
#define CHECK_SETUP_IN(type)     if ((setup->wLength != type) \
        ||(setup->Direction != USB_SETUP_DIR_IN) \
        ||(setup->Recipient != USB_SETUP_RECIPIENT_DEVICE)) { \
        return USB_STATUS_REQ_ERR; \
    }

#define CHECK_SETUP_OUT(type)     if ((setup->wLength != type) \
        ||(setup->Direction != USB_SETUP_DIR_OUT) \
        ||(setup->Recipient != USB_SETUP_RECIPIENT_DEVICE)) { \
        return USB_STATUS_REQ_ERR; \
    }

void state_change_cb(USBD_State_TypeDef old, USBD_State_TypeDef new);
int setup_cmd_cb(const USB_Setup_TypeDef *setup);

#endif
