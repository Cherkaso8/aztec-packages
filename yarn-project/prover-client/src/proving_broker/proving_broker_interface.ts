import {
  type ProofUri,
  type ProvingJob,
  type ProvingJobId,
  type ProvingJobSettledResult,
  type ProvingJobStatus,
  type ProvingRequestType,
} from '@aztec/circuit-types';

/**
 * An interface for the proving orchestrator. The producer uses this to enqueue jobs for agents
 */
export interface ProvingJobProducer {
  /**
   * Enqueues a proving job
   * @param job - The job to enqueue
   */
  enqueueProvingJob(job: ProvingJob): Promise<void>;

  /**
   * Cancels a proving job and clears all of its
   * @param id - The ID of the job to cancel
   */
  removeAndCancelProvingJob(id: ProvingJobId): Promise<void>;

  /**
   * Returns the current status fof the proving job
   * @param id - The ID of the job to get the status of
   */
  getProvingJobStatus(id: ProvingJobId): Promise<ProvingJobStatus>;

  /**
   * Waits for the job to settle and returns to the result
   * @param id - The ID of the job to get the status of
   */
  waitForJobToSettle(id: ProvingJobId): Promise<ProvingJobSettledResult>;
}

export interface ProvingJobFilter<T extends ProvingRequestType[]> {
  allowList?: T;
}

/**
 * An interface for proving agents to request jobs and report results
 */
export interface ProvingJobConsumer {
  /**
   * Gets a proving job to work on
   * @param filter - Optional filter for the type of job to get
   */
  getProvingJob<T extends ProvingRequestType[]>(
    filter?: ProvingJobFilter<T>,
  ): Promise<{ job: ProvingJob; time: number } | undefined>;

  /**
   * Marks a proving job as successful
   * @param id - The ID of the job to report success for
   * @param result - The result of the job
   */
  reportProvingJobSuccess(id: ProvingJobId, result: ProofUri): Promise<void>;

  /**
   * Marks a proving job as errored
   * @param id - The ID of the job to report an error for
   * @param err - The error that occurred while processing the job
   * @param retry - Whether to retry the job
   */
  reportProvingJobError(id: ProvingJobId, err: Error, retry?: boolean): Promise<void>;

  /**
   * Sends a heartbeat to the broker to indicate that the agent is still working on the given proving job
   * @param id - The ID of the job to report progress for
   * @param startedAt - The unix epoch when the job was started
   * @param filter - Optional filter for the type of job to get
   */
  reportProvingJobProgress<F extends ProvingRequestType[]>(
    id: ProvingJobId,
    startedAt: number,
    filter?: ProvingJobFilter<F>,
  ): Promise<{ job: ProvingJob; time: number } | undefined>;
}