#pragma once
#include "barretenberg/crypto/merkle_tree/indexed_tree/indexed_leaf.hpp"
#include "barretenberg/ecc/curves/bn254/fr.hpp"
#include "barretenberg/messaging/header.hpp"
#include "barretenberg/serialize/msgpack.hpp"
#include "barretenberg/world_state/types.hpp"
#include "barretenberg/world_state/world_state.hpp"
#include <cstdint>
#include <string>

namespace bb::world_state {

using namespace bb::messaging;

enum WorldStateMessageType {
    GET_TREE_INFO = FIRST_APP_MSG_TYPE,
    GET_STATE_REFERENCE,
    GET_INITIAL_STATE_REFERENCE,

    GET_LEAF_VALUE,
    GET_LEAF_PREIMAGE,
    GET_SIBLING_PATH,

    FIND_LEAF_INDEX,
    FIND_LOW_LEAF,

    APPEND_LEAVES,
    BATCH_INSERT,

    UPDATE_ARCHIVE,

    COMMIT,
    ROLLBACK,

    SYNC_BLOCK,

    CREATE_FORK,
    DELETE_FORK,
};

struct TreeIdOnlyRequest {
    MerkleTreeId treeId;
    MSGPACK_FIELDS(treeId);
};

struct CreateForkRequest {
    uint64_t blockNumber;
    MSGPACK_FIELDS(blockNumber);
};

struct CreateForkResponse {
    uint64_t forkId;
    MSGPACK_FIELDS(forkId);
};

struct DeleteForkRequest {
    uint64_t forkId;
    MSGPACK_FIELDS(forkId);
};

struct TreeIdAndRevisionRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    MSGPACK_FIELDS(treeId, revision);
};

struct EmptyResponse {
    bool ok{ true };
    MSGPACK_FIELDS(ok);
};

struct GetTreeInfoRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    MSGPACK_FIELDS(treeId, revision);
};

struct GetTreeInfoResponse {
    MerkleTreeId treeId;
    fr root;
    index_t size;
    uint32_t depth;
    MSGPACK_FIELDS(treeId, root, size, depth);
};

struct GetStateReferenceRequest {
    WorldStateRevision revision;
    MSGPACK_FIELDS(revision);
};

struct GetStateReferenceResponse {
    StateReference state;
    MSGPACK_FIELDS(state);
};

struct GetInitialStateReferenceResponse {
    StateReference state;
    MSGPACK_FIELDS(state);
};

struct GetLeafValueRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    index_t leafIndex;
    MSGPACK_FIELDS(treeId, revision, leafIndex);
};

struct GetLeafPreimageRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    index_t leafIndex;
    MSGPACK_FIELDS(treeId, revision, leafIndex);
};

struct GetSiblingPathRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    index_t leafIndex;
    MSGPACK_FIELDS(treeId, revision, leafIndex);
};

template <typename T> struct FindLeafIndexRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    T leaf;
    MSGPACK_FIELDS(treeId, revision, leaf);
};

struct FindLowLeafRequest {
    MerkleTreeId treeId;
    WorldStateRevision revision;
    fr key;
    MSGPACK_FIELDS(treeId, revision, key);
};

struct FindLowLeafResponse {
    bool alreadyPresent;
    index_t index;
    MSGPACK_FIELDS(alreadyPresent, index);
};

template <typename T> struct AppendLeavesRequest {
    MerkleTreeId treeId;
    std::vector<T> leaves;
    MSGPACK_FIELDS(treeId, leaves);
};

template <typename T> struct BatchInsertRequest {
    MerkleTreeId treeId;
    std::vector<T> leaves;
    uint32_t subtreeDepth;
    MSGPACK_FIELDS(treeId, leaves, subtreeDepth);
};

struct UpdateArchiveRequest {
    StateReference blockStateRef;
    bb::fr blockHash;
    MSGPACK_FIELDS(blockStateRef, blockHash);
};

struct SyncBlockRequest {
    StateReference blockStateRef;
    bb::fr blockHash;
    std::vector<bb::fr> paddedNoteHashes, paddedL1ToL2Messages;
    std::vector<crypto::merkle_tree::NullifierLeafValue> paddedNullifiers;
    std::vector<std::vector<crypto::merkle_tree::PublicDataLeafValue>> batchesOfPaddedPublicDataWrites;

    MSGPACK_FIELDS(blockStateRef,
                   blockHash,
                   paddedNoteHashes,
                   paddedL1ToL2Messages,
                   paddedNullifiers,
                   batchesOfPaddedPublicDataWrites);
};

struct SyncBlockResponse {
    bool isBlockOurs;
    MSGPACK_FIELDS(isBlockOurs);
};

} // namespace bb::world_state

MSGPACK_ADD_ENUM(bb::world_state::WorldStateMessageType)