#include "../include/timedelta.h"

struct timedelta_buf* timedelta_buf_new(long days, long seconds, long microseconds) {
    struct timedelta_buf* td_buf = (struct timedelta_buf*)PyMem_RawMalloc(3 * sizeof(char) * 4);
    if (td_buf == NULL) {
        return NULL;
    }
    memset(td_buf->buf, days, 4 * sizeof(char));
    memset(td_buf->buf + 4, seconds, 4 * sizeof(char));
    memset(td_buf->buf + 8, microseconds, 4 * sizeof(char));
    return td_buf;
}

void timedelta_buf_delete(struct timedelta_buf* td_buf) {
     PyMem_RawFree(td_buf);
}

long timedelta_buf_get_days(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[0] << 24 | td_buf->buf[1] << 16 | td_buf->buf[2] << 8 | td_buf->buf[3]);
}

long timedelta_buf_get_seconds(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[4] << 24 | td_buf->buf[5] << 16 | td_buf->buf[6] << 8 | td_buf->buf[7]);
}

long timedelta_buf_get_microseconds(struct timedelta_buf* td_buf) {
    return (long)(td_buf->buf[8] << 24 | td_buf->buf[9] << 16 | td_buf->buf[10] << 8 | td_buf->buf[11]);
}

void TimedeltaExporter_dealloc(PyObject* self) {
    timedelta_buf_delete(((TimedeltaExporter*)self)->timedelta);
    Py_TYPE(self)->tp_free(self);
}

