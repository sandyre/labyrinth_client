    // automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MSNET_MSNET_H_
#define FLATBUFFERS_GENERATED_MSNET_MSNET_H_

#include <mflatbuffers/flatbuffers.h>


namespace MasterEvent {
    
    struct CLPing;
    
    struct SVPing;
    
    struct CLRegister;
    
    struct SVRegister;
    
    struct CLLogin;
    
    struct SVLogin;
    
    struct CLFindGame;
    
    struct SVFindGame;
    
    struct SVGameFound;
    
    struct Message;
    
    enum RegistrationStatus {
        RegistrationStatus_SUCCESS = 0,
        RegistrationStatus_EMAIL_TAKEN = 1,
        RegistrationStatus_MIN = RegistrationStatus_SUCCESS,
        RegistrationStatus_MAX = RegistrationStatus_EMAIL_TAKEN
    };
    
    inline const char **EnumNamesRegistrationStatus() {
        static const char *names[] = {
            "SUCCESS",
            "EMAIL_TAKEN",
            nullptr
        };
        return names;
    }
    
    inline const char *EnumNameRegistrationStatus(RegistrationStatus e) {
        const size_t index = static_cast<int>(e);
        return EnumNamesRegistrationStatus()[index];
    }
    
    enum LoginStatus {
        LoginStatus_SUCCESS = 0,
        LoginStatus_WRONG_INPUT = 1,
        LoginStatus_MIN = LoginStatus_SUCCESS,
        LoginStatus_MAX = LoginStatus_WRONG_INPUT
    };
    
    inline const char **EnumNamesLoginStatus() {
        static const char *names[] = {
            "SUCCESS",
            "WRONG_INPUT",
            nullptr
        };
        return names;
    }
    
    inline const char *EnumNameLoginStatus(LoginStatus e) {
        const size_t index = static_cast<int>(e);
        return EnumNamesLoginStatus()[index];
    }
    
    enum ConnectionResponse {
        ConnectionResponse_ACCEPTED = 0,
        ConnectionResponse_REFUSED = 1,
        ConnectionResponse_MIN = ConnectionResponse_ACCEPTED,
        ConnectionResponse_MAX = ConnectionResponse_REFUSED
    };
    
    inline const char **EnumNamesConnectionResponse() {
        static const char *names[] = {
            "ACCEPTED",
            "REFUSED",
            nullptr
        };
        return names;
    }
    
    inline const char *EnumNameConnectionResponse(ConnectionResponse e) {
        const size_t index = static_cast<int>(e);
        return EnumNamesConnectionResponse()[index];
    }
    
    enum Messages {
        Messages_NONE = 0,
        Messages_CLPing = 1,
        Messages_SVPing = 2,
        Messages_CLRegister = 3,
        Messages_SVRegister = 4,
        Messages_CLLogin = 5,
        Messages_SVLogin = 6,
        Messages_CLFindGame = 7,
        Messages_SVFindGame = 8,
        Messages_SVGameFound = 9,
        Messages_MIN = Messages_NONE,
        Messages_MAX = Messages_SVGameFound
    };
    
    inline const char **EnumNamesMessages() {
        static const char *names[] = {
            "NONE",
            "CLPing",
            "SVPing",
            "CLRegister",
            "SVRegister",
            "CLLogin",
            "SVLogin",
            "CLFindGame",
            "SVFindGame",
            "SVGameFound",
            nullptr
        };
        return names;
    }
    
    inline const char *EnumNameMessages(Messages e) {
        const size_t index = static_cast<int>(e);
        return EnumNamesMessages()[index];
    }
    
    template<typename T> struct MessagesTraits {
        static const Messages enum_value = Messages_NONE;
    };
    
    template<> struct MessagesTraits<CLPing> {
        static const Messages enum_value = Messages_CLPing;
    };
    
    template<> struct MessagesTraits<SVPing> {
        static const Messages enum_value = Messages_SVPing;
    };
    
    template<> struct MessagesTraits<CLRegister> {
        static const Messages enum_value = Messages_CLRegister;
    };
    
    template<> struct MessagesTraits<SVRegister> {
        static const Messages enum_value = Messages_SVRegister;
    };
    
    template<> struct MessagesTraits<CLLogin> {
        static const Messages enum_value = Messages_CLLogin;
    };
    
    template<> struct MessagesTraits<SVLogin> {
        static const Messages enum_value = Messages_SVLogin;
    };
    
    template<> struct MessagesTraits<CLFindGame> {
        static const Messages enum_value = Messages_CLFindGame;
    };
    
    template<> struct MessagesTraits<SVFindGame> {
        static const Messages enum_value = Messages_SVFindGame;
    };
    
    template<> struct MessagesTraits<SVGameFound> {
        static const Messages enum_value = Messages_SVGameFound;
    };
    
    bool VerifyMessages(flatbuffers::Verifier &verifier, const void *obj, Messages type);
    bool VerifyMessagesVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);
    
    struct CLPing FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            verifier.EndTable();
        }
    };
    
    struct CLPingBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        CLPingBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        CLPingBuilder &operator=(const CLPingBuilder &);
        flatbuffers::Offset<CLPing> Finish() {
            const auto end = fbb_.EndTable(start_, 0);
            auto o = flatbuffers::Offset<CLPing>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<CLPing> CreateCLPing(
                                                    flatbuffers::FlatBufferBuilder &_fbb) {
        CLPingBuilder builder_(_fbb);
        return builder_.Finish();
    }
    
    struct SVPing FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            verifier.EndTable();
        }
    };
    
    struct SVPingBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        SVPingBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        SVPingBuilder &operator=(const SVPingBuilder &);
        flatbuffers::Offset<SVPing> Finish() {
            const auto end = fbb_.EndTable(start_, 0);
            auto o = flatbuffers::Offset<SVPing>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<SVPing> CreateSVPing(
                                                    flatbuffers::FlatBufferBuilder &_fbb) {
        SVPingBuilder builder_(_fbb);
        return builder_.Finish();
    }
    
    struct CLRegister FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_EMAIL = 4,
            VT_PASSWORD = 6
        };
        const flatbuffers::String *email() const {
            return GetPointer<const flatbuffers::String *>(VT_EMAIL);
        }
        const flatbuffers::String *password() const {
            return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_EMAIL) &&
            verifier.Verify(email()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_PASSWORD) &&
            verifier.Verify(password()) &&
            verifier.EndTable();
        }
    };
    
    struct CLRegisterBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_email(flatbuffers::Offset<flatbuffers::String> email) {
            fbb_.AddOffset(CLRegister::VT_EMAIL, email);
        }
        void add_password(flatbuffers::Offset<flatbuffers::String> password) {
            fbb_.AddOffset(CLRegister::VT_PASSWORD, password);
        }
        CLRegisterBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        CLRegisterBuilder &operator=(const CLRegisterBuilder &);
        flatbuffers::Offset<CLRegister> Finish() {
            const auto end = fbb_.EndTable(start_, 2);
            auto o = flatbuffers::Offset<CLRegister>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<CLRegister> CreateCLRegister(
                                                            flatbuffers::FlatBufferBuilder &_fbb,
                                                            flatbuffers::Offset<flatbuffers::String> email = 0,
                                                            flatbuffers::Offset<flatbuffers::String> password = 0) {
        CLRegisterBuilder builder_(_fbb);
        builder_.add_password(password);
        builder_.add_email(email);
        return builder_.Finish();
    }
    
    inline flatbuffers::Offset<CLRegister> CreateCLRegisterDirect(
                                                                  flatbuffers::FlatBufferBuilder &_fbb,
                                                                  const char *email = nullptr,
                                                                  const char *password = nullptr) {
        return CreateCLRegister(
                                _fbb,
                                email ? _fbb.CreateString(email) : 0,
                                password ? _fbb.CreateString(password) : 0);
    }
    
    struct SVRegister FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_RESPONSE = 4
        };
        RegistrationStatus response() const {
            return static_cast<RegistrationStatus>(GetField<int8_t>(VT_RESPONSE, 0));
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<int8_t>(verifier, VT_RESPONSE) &&
            verifier.EndTable();
        }
    };
    
    struct SVRegisterBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_response(RegistrationStatus response) {
            fbb_.AddElement<int8_t>(SVRegister::VT_RESPONSE, static_cast<int8_t>(response), 0);
        }
        SVRegisterBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        SVRegisterBuilder &operator=(const SVRegisterBuilder &);
        flatbuffers::Offset<SVRegister> Finish() {
            const auto end = fbb_.EndTable(start_, 1);
            auto o = flatbuffers::Offset<SVRegister>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<SVRegister> CreateSVRegister(
                                                            flatbuffers::FlatBufferBuilder &_fbb,
                                                            RegistrationStatus response = RegistrationStatus_SUCCESS) {
        SVRegisterBuilder builder_(_fbb);
        builder_.add_response(response);
        return builder_.Finish();
    }
    
    struct CLLogin FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_EMAIL = 4,
            VT_PASSWORD = 6
        };
        const flatbuffers::String *email() const {
            return GetPointer<const flatbuffers::String *>(VT_EMAIL);
        }
        const flatbuffers::String *password() const {
            return GetPointer<const flatbuffers::String *>(VT_PASSWORD);
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_EMAIL) &&
            verifier.Verify(email()) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_PASSWORD) &&
            verifier.Verify(password()) &&
            verifier.EndTable();
        }
    };
    
    struct CLLoginBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_email(flatbuffers::Offset<flatbuffers::String> email) {
            fbb_.AddOffset(CLLogin::VT_EMAIL, email);
        }
        void add_password(flatbuffers::Offset<flatbuffers::String> password) {
            fbb_.AddOffset(CLLogin::VT_PASSWORD, password);
        }
        CLLoginBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        CLLoginBuilder &operator=(const CLLoginBuilder &);
        flatbuffers::Offset<CLLogin> Finish() {
            const auto end = fbb_.EndTable(start_, 2);
            auto o = flatbuffers::Offset<CLLogin>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<CLLogin> CreateCLLogin(
                                                      flatbuffers::FlatBufferBuilder &_fbb,
                                                      flatbuffers::Offset<flatbuffers::String> email = 0,
                                                      flatbuffers::Offset<flatbuffers::String> password = 0) {
        CLLoginBuilder builder_(_fbb);
        builder_.add_password(password);
        builder_.add_email(email);
        return builder_.Finish();
    }
    
    inline flatbuffers::Offset<CLLogin> CreateCLLoginDirect(
                                                            flatbuffers::FlatBufferBuilder &_fbb,
                                                            const char *email = nullptr,
                                                            const char *password = nullptr) {
        return CreateCLLogin(
                             _fbb,
                             email ? _fbb.CreateString(email) : 0,
                             password ? _fbb.CreateString(password) : 0);
    }
    
    struct SVLogin FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_RESPONSE = 4
        };
        LoginStatus response() const {
            return static_cast<LoginStatus>(GetField<int8_t>(VT_RESPONSE, 0));
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<int8_t>(verifier, VT_RESPONSE) &&
            verifier.EndTable();
        }
    };
    
    struct SVLoginBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_response(LoginStatus response) {
            fbb_.AddElement<int8_t>(SVLogin::VT_RESPONSE, static_cast<int8_t>(response), 0);
        }
        SVLoginBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        SVLoginBuilder &operator=(const SVLoginBuilder &);
        flatbuffers::Offset<SVLogin> Finish() {
            const auto end = fbb_.EndTable(start_, 1);
            auto o = flatbuffers::Offset<SVLogin>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<SVLogin> CreateSVLogin(
                                                      flatbuffers::FlatBufferBuilder &_fbb,
                                                      LoginStatus response = LoginStatus_SUCCESS) {
        SVLoginBuilder builder_(_fbb);
        builder_.add_response(response);
        return builder_.Finish();
    }
    
    struct CLFindGame FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_PLAYER_UID = 4,
            VT_CL_VERSION_MAJOR = 6,
            VT_CL_VERSION_MINOR = 8,
            VT_CL_VERSION_BUILD = 10
        };
        uint32_t player_uid() const {
            return GetField<uint32_t>(VT_PLAYER_UID, 0);
        }
        int8_t cl_version_major() const {
            return GetField<int8_t>(VT_CL_VERSION_MAJOR, 0);
        }
        int8_t cl_version_minor() const {
            return GetField<int8_t>(VT_CL_VERSION_MINOR, 0);
        }
        int8_t cl_version_build() const {
            return GetField<int8_t>(VT_CL_VERSION_BUILD, 0);
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint32_t>(verifier, VT_PLAYER_UID) &&
            VerifyField<int8_t>(verifier, VT_CL_VERSION_MAJOR) &&
            VerifyField<int8_t>(verifier, VT_CL_VERSION_MINOR) &&
            VerifyField<int8_t>(verifier, VT_CL_VERSION_BUILD) &&
            verifier.EndTable();
        }
    };
    
    struct CLFindGameBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_player_uid(uint32_t player_uid) {
            fbb_.AddElement<uint32_t>(CLFindGame::VT_PLAYER_UID, player_uid, 0);
        }
        void add_cl_version_major(int8_t cl_version_major) {
            fbb_.AddElement<int8_t>(CLFindGame::VT_CL_VERSION_MAJOR, cl_version_major, 0);
        }
        void add_cl_version_minor(int8_t cl_version_minor) {
            fbb_.AddElement<int8_t>(CLFindGame::VT_CL_VERSION_MINOR, cl_version_minor, 0);
        }
        void add_cl_version_build(int8_t cl_version_build) {
            fbb_.AddElement<int8_t>(CLFindGame::VT_CL_VERSION_BUILD, cl_version_build, 0);
        }
        CLFindGameBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        CLFindGameBuilder &operator=(const CLFindGameBuilder &);
        flatbuffers::Offset<CLFindGame> Finish() {
            const auto end = fbb_.EndTable(start_, 4);
            auto o = flatbuffers::Offset<CLFindGame>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<CLFindGame> CreateCLFindGame(
                                                            flatbuffers::FlatBufferBuilder &_fbb,
                                                            uint32_t player_uid = 0,
                                                            int8_t cl_version_major = 0,
                                                            int8_t cl_version_minor = 0,
                                                            int8_t cl_version_build = 0) {
        CLFindGameBuilder builder_(_fbb);
        builder_.add_player_uid(player_uid);
        builder_.add_cl_version_build(cl_version_build);
        builder_.add_cl_version_minor(cl_version_minor);
        builder_.add_cl_version_major(cl_version_major);
        return builder_.Finish();
    }
    
    struct SVFindGame FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_PLAYER_UID = 4,
            VT_RESPONSE = 6
        };
        uint32_t player_uid() const {
            return GetField<uint32_t>(VT_PLAYER_UID, 0);
        }
        ConnectionResponse response() const {
            return static_cast<ConnectionResponse>(GetField<int8_t>(VT_RESPONSE, 0));
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint32_t>(verifier, VT_PLAYER_UID) &&
            VerifyField<int8_t>(verifier, VT_RESPONSE) &&
            verifier.EndTable();
        }
    };
    
    struct SVFindGameBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_player_uid(uint32_t player_uid) {
            fbb_.AddElement<uint32_t>(SVFindGame::VT_PLAYER_UID, player_uid, 0);
        }
        void add_response(ConnectionResponse response) {
            fbb_.AddElement<int8_t>(SVFindGame::VT_RESPONSE, static_cast<int8_t>(response), 0);
        }
        SVFindGameBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        SVFindGameBuilder &operator=(const SVFindGameBuilder &);
        flatbuffers::Offset<SVFindGame> Finish() {
            const auto end = fbb_.EndTable(start_, 2);
            auto o = flatbuffers::Offset<SVFindGame>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<SVFindGame> CreateSVFindGame(
                                                            flatbuffers::FlatBufferBuilder &_fbb,
                                                            uint32_t player_uid = 0,
                                                            ConnectionResponse response = ConnectionResponse_ACCEPTED) {
        SVFindGameBuilder builder_(_fbb);
        builder_.add_player_uid(player_uid);
        builder_.add_response(response);
        return builder_.Finish();
    }
    
    struct SVGameFound FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_GS_PORT = 4
        };
        uint32_t gs_port() const {
            return GetField<uint32_t>(VT_GS_PORT, 0);
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint32_t>(verifier, VT_GS_PORT) &&
            verifier.EndTable();
        }
    };
    
    struct SVGameFoundBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_gs_port(uint32_t gs_port) {
            fbb_.AddElement<uint32_t>(SVGameFound::VT_GS_PORT, gs_port, 0);
        }
        SVGameFoundBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        SVGameFoundBuilder &operator=(const SVGameFoundBuilder &);
        flatbuffers::Offset<SVGameFound> Finish() {
            const auto end = fbb_.EndTable(start_, 1);
            auto o = flatbuffers::Offset<SVGameFound>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<SVGameFound> CreateSVGameFound(
                                                              flatbuffers::FlatBufferBuilder &_fbb,
                                                              uint32_t gs_port = 0) {
        SVGameFoundBuilder builder_(_fbb);
        builder_.add_gs_port(gs_port);
        return builder_.Finish();
    }
    
    struct Message FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
        enum {
            VT_MESSAGE_TYPE = 4,
            VT_MESSAGE = 6
        };
        Messages message_type() const {
            return static_cast<Messages>(GetField<uint8_t>(VT_MESSAGE_TYPE, 0));
        }
        const void *message() const {
            return GetPointer<const void *>(VT_MESSAGE);
        }
        bool Verify(flatbuffers::Verifier &verifier) const {
            return VerifyTableStart(verifier) &&
            VerifyField<uint8_t>(verifier, VT_MESSAGE_TYPE) &&
            VerifyField<flatbuffers::uoffset_t>(verifier, VT_MESSAGE) &&
            VerifyMessages(verifier, message(), message_type()) &&
            verifier.EndTable();
        }
    };
    
    struct MessageBuilder {
        flatbuffers::FlatBufferBuilder &fbb_;
        flatbuffers::uoffset_t start_;
        void add_message_type(Messages message_type) {
            fbb_.AddElement<uint8_t>(Message::VT_MESSAGE_TYPE, static_cast<uint8_t>(message_type), 0);
        }
        void add_message(flatbuffers::Offset<void> message) {
            fbb_.AddOffset(Message::VT_MESSAGE, message);
        }
        MessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
            start_ = fbb_.StartTable();
        }
        MessageBuilder &operator=(const MessageBuilder &);
        flatbuffers::Offset<Message> Finish() {
            const auto end = fbb_.EndTable(start_, 2);
            auto o = flatbuffers::Offset<Message>(end);
            return o;
        }
    };
    
    inline flatbuffers::Offset<Message> CreateMessage(
                                                      flatbuffers::FlatBufferBuilder &_fbb,
                                                      Messages message_type = Messages_NONE,
                                                      flatbuffers::Offset<void> message = 0) {
        MessageBuilder builder_(_fbb);
        builder_.add_message(message);
        builder_.add_message_type(message_type);
        return builder_.Finish();
    }
    
    inline bool VerifyMessages(flatbuffers::Verifier &verifier, const void *obj, Messages type) {
        switch (type) {
            case Messages_NONE: {
                return true;
            }
            case Messages_CLPing: {
                auto ptr = reinterpret_cast<const CLPing *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_SVPing: {
                auto ptr = reinterpret_cast<const SVPing *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_CLRegister: {
                auto ptr = reinterpret_cast<const CLRegister *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_SVRegister: {
                auto ptr = reinterpret_cast<const SVRegister *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_CLLogin: {
                auto ptr = reinterpret_cast<const CLLogin *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_SVLogin: {
                auto ptr = reinterpret_cast<const SVLogin *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_CLFindGame: {
                auto ptr = reinterpret_cast<const CLFindGame *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_SVFindGame: {
                auto ptr = reinterpret_cast<const SVFindGame *>(obj);
                return verifier.VerifyTable(ptr);
            }
            case Messages_SVGameFound: {
                auto ptr = reinterpret_cast<const SVGameFound *>(obj);
                return verifier.VerifyTable(ptr);
            }
            default: return false;
        }
    }
    
    inline bool VerifyMessagesVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
        if (values->size() != types->size()) return false;
        for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
            if (!VerifyMessages(
                                verifier,  values->Get(i), types->GetEnum<Messages>(i))) {
                return false;
            }
        }
        return true;
    }
    
    inline const MasterEvent::Message *GetMessage(const void *buf) {
        return flatbuffers::GetRoot<MasterEvent::Message>(buf);
    }
    
    inline bool VerifyMessageBuffer(
                                    flatbuffers::Verifier &verifier) {
        return verifier.VerifyBuffer<MasterEvent::Message>(nullptr);
    }
    
    inline void FinishMessageBuffer(
                                    flatbuffers::FlatBufferBuilder &fbb,
                                    flatbuffers::Offset<MasterEvent::Message> root) {
        fbb.Finish(root);
    }
    
}  // namespace MasterEvent

#endif  // FLATBUFFERS_GENERATED_MSNET_MASTEREVENT_H_
