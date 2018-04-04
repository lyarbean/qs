Paths and data layouts
======================

TickInfo
--------

a. Data:
    /TickInfo\_**Gateway**\_**Ticker**

    An array of ShmTickInfo

b. Description:
    /TickInfo\_**Gateway**\_**Ticker**\_meta

| Field                |     Description       |
|:---------------------|:----------------------|
| size of ShmTickInfo  | 4Bytes, in byte  |
| length of Data       | 4Bytes, in byte  |
| cursor               | 4Bytes, in byte, points to next position to write |
| flag                 | 4Bytes, an interger, used in future |

c. Notification

When a gateway writes a tickinfo into shm, it acquires a read-write-lock (resists on SHM) first, in order to protect Description (blocks other readers, like SharedMemeoryGateway).
Then it populates into desired memory address and updates **Description**.

As long as **Data** updates, **SharedMemeoryGateway** that epolls on **Data** receives a notification and then reads **Description** after acquiring that read-write-lock. And then it notifies its associated strategies.

For strategy, it requires no lock to access **Data**, due to the fact that **Data** is grown up by Gateway but never gets shrunk at the present design.



RequestInfo
-----------

a. Data:
    /RequestInfo\_**Gateway**\_**Ticker**

    An array of ShmRequestInfo

b. Description:
   /RequestInfo\_**Gateway**\_**Ticker**\_meta

| Field                |     Description       |
|:---------------------|:----------------------|
| size of ShmRequestInfo  | 4Bytes, in byte  |
| length of Data       | 4Bytes, in byte  |
| cursor               | 4Bytes, in byte, points to next position to write |
| flag                 | 4Bytes, an interger, used in future |




