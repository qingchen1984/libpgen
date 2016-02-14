

#pragma once


#include <assert.h>
#include <pgen2/io/util.h>
#include <pgen2/types/data_container.h>




namespace pgen {
namespace core {


enum class packet_type {
    unknown,
    ethernet, 
    arp,
    ip,
    icmp,
    tcp,
    udp
};




class packet {
    protected:
        pgen::core::packet_type _type;
        pgen::types::data_container  _raw;
        size_t _header_len;

        virtual void compile() = 0;

    public:
        static const size_t max_header_len = 128; /* [byte] */

        /* For Packet Contents */
        void  set_contents(const void* buffer, size_t buflen);
        const uint8_t* contents() const;
        
        /* For Raw datas */
        const uint8_t* raw() const;
        size_t length() const;
        virtual size_t header_length() const = 0;

        packet_type get_type() const;
        void hex() const;

        virtual void clear() = 0;
        virtual void analyze(const void* buffer, size_t buffer_len) = 0;

        virtual void summary(bool moreinfo=false) const = 0;
};









} /* namespace core */
} /* namespace pgen */

